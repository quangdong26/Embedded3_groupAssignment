#include "./utils.h"
// #include "font.h"
// #include "../uart/uart0.h"
// /* Functions to display text on the screen */

// void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom){
//     unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

//     for (int i = 1; i <= (FONT_HEIGHT*zoom); i++) {
// 		for (int j = 0; j< (FONT_WIDTH*zoom); j++) {
// 			unsigned char mask = 1 << (j/zoom);
//             if (*glyph & mask) { //only draw pixels belong to the character glyph
// 			    drawPixelARGB32(x + j, y + i, attr);
//             }
// 		}
// 		glyph += (i % zoom) ? 0 : FONT_BPL;
//     }
// }


// void drawString(int x, int y, char *str, unsigned int attr, int zoom)
// {
//     while (*str) {
//         if (*str == '\r') {
//             x = 0;
//         } else if (*str == '\n') {
//             x = 0; 
// 			y += (FONT_HEIGHT*zoom);
//         } else {
//             drawChar(*str, x, y, attr, zoom);
//             x += (FONT_WIDTH*zoom);
//         }
//         str++;
//     }
// }



// /* New function for Uart: Check and return if no new character, don't wait */

// #if 1 //UART0
// unsigned int uart_isReadByteReady(){
// 	return ( !(UART0_FR & UART0_FR_RXFE) );
// }

// #else //UART1
// unsigned int uart_isReadByteReady(){
// 	return (AUX_MU_LSR & 0x01);
// }
// #endif

// unsigned char getUart(){
//     unsigned char ch = 0;
//     if (uart_isReadByteReady())
//     	ch = uart_getc();
//     return ch;
// }



// /* Functions to delay, set/wait timer */

// void wait_msec(unsigned int n)
// {
//     register unsigned long f, t, r, expiredTime;

//     // Get the current counter frequency (Hz)
//     asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    
//     // Read the current counter value
//     asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    
//     // Calculate expire value for counter
//     expiredTime = t + ( (f/1000)*n )/1000;
//     do {
//     	asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
//     } while(r < expiredTime);
// }



// void set_wait_timer(int set, unsigned int msVal) {
//     static unsigned long expiredTime = 0; //declare static to keep value
//     register unsigned long r, f, t;
    
//     if (set) { /* SET TIMER */
//         // Get the current counter frequency (Hz)
//         asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));

//         // Read the current counter
//         asm volatile ("mrs %0, cntpct_el0" : "=r"(t));

//         // Calculate expired time:
//         expiredTime = t + ( (f/1000)*msVal )/1000;
//     } 
//     else { /* WAIT FOR TIMER TO EXPIRE */
//         do {
//             asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
//         } while(r < expiredTime);
//     }
// }

static unsigned int lcg_seed = 123456789; // Initial seed
int obstacle_ctr = 0;
int obstacle_width = 0;
int obstacle_height = 0;

unsigned int lcg_random() {
    lcg_seed = (LCG_A * lcg_seed + LCG_C) % LCG_M;
    return lcg_seed;
}

void convert1DTo2D(int *one_d_array, int rows, int cols, int two_d_array[rows][cols]) {
    // Fill the 2D array from the 1D array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            two_d_array[i][j] = one_d_array[i * cols + j];
        }
    }
}

void deleteColumns(unsigned long original[][6], unsigned long edited[][3], int rows, int cols, int colStart, int colEnd) {
    for (int i = 0; i < rows; i++) {
        int newCol = 0; // Column index for the new array
        for (int j = 0; j < cols; j++) {
            // Check if the current column is within the range to delete
            if (j < colStart || j > colEnd) {
                edited[i][newCol] = original[i][j];
                newCol++;
            }
        }
    }
}

// Function to get the number of columns in a 2D array
int getNumberOfColumns(unsigned long array[][5], int rows) {
    // Since the array is passed with a fixed second dimension (5 in this case),
    // we can use the size of one row to calculate the number of columns.
    int cols = sizeof(array[0]) / sizeof(array[0][0]);
    return cols;
}

int abs(int x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

// Function to check if a pixel is the background color
int is_background(unsigned long pixel) {
    return pixel == 0x00000000;
}


// Function to detect obstacles in the image and return their dimensions
int detect_obstacles(const unsigned long *image, int width, int height) {
    int max_row = 0;
    int detected[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            detected[i][j] = 0;
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            if (!is_background(image[index]) && !detected[y][x] &&
                (image[index] == GREEN_PIXEL || image[index] == LIGHT_GREEN_PIXEL)) {

                int start_x = x, start_y = y;
                int end_x = x, end_y = y;

                detected[y][x] = 1;

                // Mark the entire obstacle as detected and find its bounds
                for (int j = y; j < height; j++) {
                    for (int i = x; i < width; i++) {
                        int new_index = j * width + i;
                        if (!is_background(image[new_index]) &&
                            (image[new_index] == GREEN_PIXEL || image[new_index] == LIGHT_GREEN_PIXEL)) {
                            detected[j][i] = 1;
                            obstacle_ctr++;
                            if (i > end_x) end_x = i;
                            if (j > end_y) end_y = j;
                        } else {
                            if(obstacle_ctr > max_row) {
                                max_row = obstacle_ctr;
                            }
                            obstacle_ctr = 0;
                            obstacle_height++;
                            break;
                        }
                    }
                    if (is_background(image[j * width + x]) ||
                        !(image[j * width + x] == GREEN_PIXEL || image[j * width + x] == LIGHT_GREEN_PIXEL)) {
                        break;
                    }
                }
                obstacle_width = end_x - start_x + 1;
            }
        }
    }
    return max_row;
}

