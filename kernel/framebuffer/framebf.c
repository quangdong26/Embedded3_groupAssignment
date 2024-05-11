// ----------------------------------- framebf.c -------------------------------------
#include "./framebf.h"

//Use RGBA32 (32 bits for each pixel)
#define COLOR_DEPTH 32
//Pixel Order: BGR in memory order (little endian --> RGB in byte order)
#define PIXEL_ORDER 0
//Screen info
unsigned int width, height, pitch;
unsigned int win_wid = 1024; //window width
unsigned int win_hei = 768; // window height
/* Frame buffer address
* (declare as pointer of unsigned char to access each byte) */
unsigned char *fb;
/**
* Set screen resolution to 1024x768
*/
void framebf_init()
{
mBuf[0] = 35*4; // Length of message in bytes
mBuf[1] = MBOX_REQUEST;
mBuf[2] = MBOX_TAG_SETPHYWH; //Set physical width-height
mBuf[3] = 8; // Value size in bytes
mBuf[4] = 0; // REQUEST CODE = 0
mBuf[5] = win_wid; // Value(width)
mBuf[6] = win_hei; // Value(height)
mBuf[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height

mBuf[8] = 8;
mBuf[9] = 0;
mBuf[10] = win_wid;
mBuf[11] = win_hei;
mBuf[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset
mBuf[13] = 8;
mBuf[14] = 0;
mBuf[15] = 0; // x offset
mBuf[16] = 0; // y offset
mBuf[17] = MBOX_TAG_SETDEPTH; //Set color depth
mBuf[18] = 4;
mBuf[19] = 0;
mBuf[20] = COLOR_DEPTH; //Bits per pixel
mBuf[21] = MBOX_TAG_SETPXLORDR; //Set pixel order
mBuf[22] = 4;
mBuf[23] = 0;
mBuf[24] = PIXEL_ORDER;
mBuf[25] = MBOX_TAG_GETFB; //Get frame buffer
mBuf[26] = 8;
mBuf[27] = 0;
mBuf[28] = 16; //alignment in 16 bytes
mBuf[29] = 0; //will return Frame Buffer size in bytes
mBuf[30] = MBOX_TAG_GETPITCH; //Get pitch
mBuf[31] = 4;
mBuf[32] = 0;
mBuf[33] = 0; //Will get pitch value here
mBuf[34] = MBOX_TAG_LAST;
// Call Mailbox
if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) //mailbox call is successful ?
&& mBuf[20] == COLOR_DEPTH //got correct color depth ?
&& mBuf[24] == PIXEL_ORDER //got correct pixel order ?
&& mBuf[28] != 0 //got a valid address for frame buffer ?
) {
/* Convert GPU address to ARM address (clear higher address bits)
* Frame Buffer is located in RAM memory, which VideoCore MMU
* maps it to bus address space starting at 0xC0000000.
* Software accessing RAM directly use physical addresses
* (based at 0x00000000)
*/
mBuf[28] &= 0x3FFFFFFF;//?0x3FFF FFFF????????
// Access frame buffer as 1 byte per each address
fb = (unsigned char *)((unsigned long)mBuf[28]);
uart_puts("Got allocated Frame Buffer at RAM physical address: ");
uart_hex(mBuf[28]);
uart_puts("\n");
uart_puts("Frame Buffer Size (bytes): ");
uart_hex(mBuf[29]); //????????????????????????????????? in running at the beginning but now have to change back to hex?
uart_puts("\n");
width = mBuf[5]; // Actual physical width
height = mBuf[6]; // Actual physical height
pitch = mBuf[33]; // Number of bytes per line
} else {
uart_puts("Unable to get a frame buffer with provided setting\n");

}
}
void drawPixelARGB32(int x, int y, unsigned int attr)
{
    int offs = (y * pitch) + (COLOR_DEPTH/8 * x);
    /* //Access and assign each byte
    *(fb + offs ) = (attr >> 0 ) & 0xFF; //BLUE (get the least significant byte)
    *(fb + offs + 1) = (attr >> 8 ) & 0xFF; //GREEN
    *(fb + offs + 2) = (attr >> 16) & 0xFF; //RED
    *(fb + offs + 3) = (attr >> 24) & 0xFF; //ALPHA
    */
    //Access 32-bit together
    *((unsigned int*)(fb + offs)) = attr;
}
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill)
{
for (int y = y1; y <= y2; y++ )
for (int x = x1; x <= x2; x++) {
if ((x == x1 || x == x2) || (y == y1 || y == y2))
drawPixelARGB32(x, y, attr);
else if (fill)
drawPixelARGB32(x, y, attr);
}
}

void drawWhiteSquare(int x, int y, int size) {
    // Draw top dashes
    drawRectARGB32(x - 1, y - 1, x + size, y - 1, 0xFFFFFFFF, 1); // Top border
    // Draw left and right borders
    drawRectARGB32(x - 1, y, x - 1, y + size, 0xFFFFFFFF, 1); // Left border
    drawRectARGB32(x + size, y, x + size, y + size, 0xFFFFFFFF, 1); // Right border
    // Draw bottom dashes
    drawRectARGB32(x - 1, y + size, x + size, y + size, 0xFFFFFFFF, 1); // Bottom border
}
void displayImage(int x, int y, const unsigned long *image, int width, int height) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // Calculate the index for the pixel data
            int index = j * width + i;
            // Draw the pixel at the corresponding (x, y) position
            drawPixelARGB32(x + i, y + j, image[index]);
        }
    }
}

void displayObject(int x, int y, const unsigned long *image, int width, int height) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // Calculate the index for the pixel data
            int index = j * width + i;
            if (image[index] == 0x00000000) {
                continue;
            }
            // Draw the pixel at the corresponding (x, y) position
            drawPixelARGB32(x + i, y + j, image[index]);
        }
    }
}

void deleteImage(int x, int y, int width, int height) { // to move the image: simply delete and replace a new image with new postion
    unsigned int backgroundColor = 0x00000000; // ARGB value for black
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // Overwrite the pixel at the corresponding (x, y) position with the background color
            drawPixelARGB32(x + i, y + j, backgroundColor);
        }
    }
}

void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom){
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i = 1; i <= (FONT_HEIGHT * zoom); i++) {
        for (int j = 0; j < (FONT_WIDTH * zoom); j++) {
            unsigned char mask = 1 << (j / zoom);
            if (*glyph & mask) { // Only draw pixels belong to the character glyph
                drawPixelARGB32(x + j, y + i, attr);
            }
        }
        glyph += (i % zoom) ? 0 : FONT_BPL;
    }
}

void drawString(int x, int y, char *str, unsigned int attr, int zoom)
{
    while (*str) {
        if (*str == '\r') {
            x = 0;
        } else if (*str == '\n') {
            x = 0; 
            y += (FONT_HEIGHT * zoom);
        } else {
            drawChar(*str, x, y, attr, zoom);
            x += (FONT_WIDTH * zoom);
        }
        str++;
    }
}


void drawPixel(int x, int y, unsigned int color)
{
    int offs = (y * pitch) + (x * 4);
    *((unsigned int*)(fb + offs)) = color;
}

void drawMario(int x, int y, unsigned int imagePixels[],int mheight, int mwidth) {
    for (int y = 0; y < mheight; y++) {
        for (int x = 0; x < mwidth; x++) {
            unsigned int pixel = imagePixels[y * mwidth + x];
            drawPixelARGB32(x, y, pixel);  
        }
    }
}
