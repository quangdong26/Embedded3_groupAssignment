#include "./game.h"
volatile int gameState = GAME_OFF;


void renderBackGround(void) {
    deleteImage(DEFAULT, DEFAULT, 3000, 3000);
    displayImage(DEFAULT, DEFAULT, background2, BACKGROUND2_WIDTH, BACKGROUND2_HEIGHT);
}

void renderPlayerInitPoint(void) {
    displayObject(0, 0, marioImg, 203, 206);
}

void gameOn(char c) {
    renderBackGround(); 
    renderPlayerInitPoint();
    switch (c)
    {
    case 'W':
        /* code */
        break;
    case 'A':
        break;
    case 'S':
        break;
    case 'D':
        break;

    default:
        break;
    }
}