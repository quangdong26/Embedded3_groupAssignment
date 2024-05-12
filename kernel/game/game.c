#include "./game.h"
volatile int gameState = GAME_OFF;
volatile int isGameInit = DEFAULT;
mario_t mario_char;


void renderBackGround(void) {
    deleteImage(DEFAULT, DEFAULT, 3000, 3000);
    displayImage(DEFAULT, DEFAULT, background2, BACKGROUND2_WIDTH, BACKGROUND2_HEIGHT);
}

void renderPlayerInitPoint(void) {
    mario_char.currentX = 0;
    mario_char.currentY = 0;
    displayObject(mario_char.currentX, mario_char.currentY, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
}

void marioJump(void) {
    mario_char.pastY = mario_char.currentY;
    mario_char.currentY -= 100;
    deleteImage(mario_char.currentX, mario_char.pastY, OBJECT_WIDTH, OBJECT_HEIGHT);
    displayObject(mario_char.currentX, mario_char.currentY, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
}

void reset(void) {
    renderBackGround(); 
    renderPlayerInitPoint();
}

void gameOn(char c) {
    // setting up the initial value for game
    if(isGameInit == DEFAULT) {
        renderBackGround(); 
        renderPlayerInitPoint();
        isGameInit = INIT;
    }

    switch (c)
    {
    case 'w':
        /* code */
        marioJump();
        break;
    case 'a':
        break;
    case 's':
        break;
    case 'd':
        break;
    case 'r': // reset
        reset();
        break;
    default:
        break;
    }

} 