#include "mbox.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "framebf.h"
#include "image.h"
#include "video.h"
#include "delay.h"

// #define MAX_CMD_SIZE 100
#define IMAGE_WIDTH 732
#define IMAGE_HEIGHT 578
#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240

int currentY = 0;
int currentX = 0;
int pastY = 0;
// Assuming fd is the file descriptor of the opened file
// buf is the buffer where the data will be stored
// count is the number of bytes to read

void cli()
{
    // static char cli_buffer[MAX_CMD_SIZE];
    // static int index = 0;
    char c = uart_getc();

    // if (c != '\n' && c != 'w' && c != 's'){ // replace 127 with '\b' if using Window
    // 	cli_buffer[index] = c; //Store into the buffer
    // 	index++;
    //     uart_sendc(c); //send c here to avoid tab appear
    // } else if (c == '\n'){
    // 	cli_buffer[index] = '\0';

    // 	uart_puts("\nGot commands: ");
    // 	uart_puts(cli_buffer);
    // }
    if (c == 'w')
    {
        pastY = currentY;
        currentY -= 100; // scroll up by 10 pixels
        deleteImage(currentX, pastY, IMAGE_WIDTH, IMAGE_HEIGHT);
        displayImage(currentX, currentY, image, IMAGE_WIDTH, IMAGE_HEIGHT);
    }
    else if (c == 's')
    {
        pastY = currentY;
        currentY += 100; // scroll down by 10 pixels
        deleteImage(currentX, pastY, IMAGE_WIDTH, IMAGE_HEIGHT);
        displayImage(currentX, currentY, image, IMAGE_WIDTH, IMAGE_HEIGHT);
    }
    else if (c == 'v')
    {
        deleteImage(currentX, currentY, IMAGE_WIDTH, IMAGE_HEIGHT);
        displayImage(0, 0, epd_bitmap_allArray[0], VIDEO_WIDTH, VIDEO_HEIGHT);
        for (int i = 1; i < 26; i++)
        {
            wait_msec(200000);
            deleteImage(0, 0, VIDEO_WIDTH,  VIDEO_HEIGHT);
            displayImage(0, 0, epd_bitmap_allArray[i] , VIDEO_WIDTH,  VIDEO_HEIGHT);
        }
    }
}

void main()
{
    // set up serial console
    uart_init();
    // say hello
    uart_puts("Hello World!\n");

    // Initialize frame buffer
    framebf_init();

    // Draw something on the screen
    // drawRectARGB32(100,100,400,400,0x00AA0000,1); //RED
    // drawRectARGB32(150,150,400,400,0x0000BB00,1); //GREEN
    // drawRectARGB32(200,200,400,400,0x000000CC,1); //BLUE
    // drawRectARGB32(250,250,400,400,0x00FFFF00,1); //YELLOW
    // drawPixelARGB32(300, 300, 0x00FF0000); //RED

    drawWhiteSquare(230, 80, 550);


    drawString(350, 100, "Team 1 : ", 0xFFFFFFFF, 4);     // white color, zoom factor of 3
    drawString(350, 200, "Shirin Shujaa", 0xFFFF99FF, 3);     // Pink color, zoom factor of 3
    drawString(350, 300, "Nguyen Ngoc Luong", 0x0000FFFF, 3); // Blue color, zoom factor of 3
    drawString(350, 400, "Hunh Quang Dong", 0x0000FF00, 3);   // Green color, zoom factor of 3
    drawString(350, 500, "Dinh Ngoc Minh", 0x00FFFF00, 3);    // Yellow color, zoom factor of 3

    // displayImage(currentX, currentY, image , IMAGE_WIDTH,  IMAGE_HEIGHT); //starting position (x,y), image, width, height

    // echo everything back
    while (1)
    {

        // //read each char
        // char c = uart_getc();
        // //send back
        // uart_sendc(c);
        cli();
    }
}