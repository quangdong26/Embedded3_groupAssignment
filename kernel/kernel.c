#include "./mailbox/mbox.h"
#include "./uart/uart0.h"
#include "./uart/uart1.h"
#include "./image/image.h"
#include "../video/video.h"
#include "./delay/delay.h"

#include "./background/background1.h"
#include "./game/game.h"




// #define MAX_CMD_SIZE 100
#define IMAGE_WIDTH 732
#define IMAGE_HEIGHT 578
#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240




int currentY = 0;
int currentX = 0;
int pastY = 0;
int pastX = 0;

int McurrentY = 0;
int McurrentX = 0;
int MpastY = 0;
int MpastX = 0;
// Assuming fd is the file descriptor of the opened file
// buf is the buffer where the data will be stored
// count is the number of bytes to read

void printTeamName(void) {
    drawWhiteSquare(230, 80, 550);


    drawString(350, 100, "Team 1 : ", 0xFFFFFFFF, 4);     // white color, zoom factor of 3
    drawString(350, 200, "Shirin Shujaa", 0xFFFF99FF, 3);     // Pink color, zoom factor of 3
    drawString(350, 300, "Nguyen Ngoc Luong", 0x0000FFFF, 3); // Blue color, zoom factor of 3
    drawString(350, 400, "Hunh Quang Dong", 0x0000FF00, 3);   // Green color, zoom factor of 3
    drawString(350, 500, "Dinh Ngoc Minh", 0x00FFFF00, 3);    // Yellow color, zoom factor of 3
}

void cli()
{
    // static char cli_buffer[MAX_CMD_SIZE];
    // static int index = 0;
    char c = uart_getc();
    if(gameState == GAME_OFF) {
        if (c == 'w')
        {
            pastY = currentY;
            currentY -= 100; // scroll up by 10 pixels
            deleteImage(currentX, pastY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            displayImage(currentX, currentY, image, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (c == 's')
        {
            pastY = currentY;
            currentY += 100; // scroll down by 10 pixels
            deleteImage(currentX, pastY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            displayImage(currentX, currentY, image, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (c == 'a')
        {
            pastX = currentX;
            currentX -= 100; // scroll down by 10 pixels
            deleteImage(pastX, currentY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            displayImage(currentX, currentY, image, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (c == 'd')
        {
            pastX = currentX;
            currentX += 100; // scroll down by 10 pixels
            deleteImage(pastX, currentY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            displayImage(currentX, currentY, image, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (c == 'v')
        {
            deleteImage(currentX, currentY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            displayImage(0, 0, epd_bitmap_allArray[0], VIDEO_WIDTH, VIDEO_HEIGHT);
            for (int i = 1; i < VIDEO_TOTAL_FRAME; i++)
            {
                wait_msec(100000);
                deleteImage(0, 0, VIDEO_WIDTH,  VIDEO_HEIGHT);
                displayImage(0, 0, epd_bitmap_allArray[i] , VIDEO_WIDTH,  VIDEO_HEIGHT);
            }
        }
        else if (c == 'g')
        {
            deleteImage(pastX, pastY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            displayImage(currentX, currentY, background1, BACKGROUND1_WIDTH, BACKGROUND1_HEIGHT);
        }
        else if (c == 'n')
        {
            deleteImage(pastX, pastY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            // displayImage(currentX, currentY, background2, BACKGROUND2_WIDTH, BACKGROUND2_HEIGHT);
        }
        else if (c == 'm') 
        {
            gameState = GAME_ON;
            // MpastX = McurrentX;
            // McurrentX += 100;
            // deleteImage(pastX, pastY, ENTIRE_SCREEN, ENTIRE_SCREEN);
            // displayImage(currentX, currentY, background2, BACKGROUND2_WIDTH, BACKGROUND2_HEIGHT);
            // displayObject(McurrentX, McurrentY, marioImg, 206, 233);
        }
    }

    if(gameState == GAME_ON) {
        gameOn(c);
    }

}

void main()
{
    // set up serial console
    uart_init();
    // say hello
    
    // Initialize frame buffer
    framebf_init(); // ini() again if you want to set the window screen to another size
    printTeamName();
    // displayImage(currentX, currentY, image , IMAGE_WIDTH,  IMAGE_HEIGHT); //starting position (x,y), image, width, height

    // echo everything back
    while (1)
    {
        cli();
    }
}