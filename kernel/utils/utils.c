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

