#include "./game.h"
volatile int gameState = GAME_OFF;
volatile int isGameInit = DEFAULT;
mario_t mario_char;
ground_t ground_obj;


void renderBackGround(void) {
    deleteImage(DEFAULT, DEFAULT, 3000, 3000);
    // displayImage(0, 0, ground, 100, 100);
    drawGround();
}

void drawGround(void) {
    ground_obj.xPos = GND_X_POS;
    ground_obj.yPos = GND_Y_POS;
    ground_obj.width = WINDOW_WIDTH - ground_obj.xPos;
    ground_obj.height = WINDOW_HEIGHT - ground_obj.yPos;

    drawArrayPixel(ground_obj.xPos, ground_obj.yPos, 0xFFA500, ground_obj.width, ground_obj.height);
}

void renderPlayerInitPoint(void) {
    mario_char.currentX = ground_obj.xPos;
    mario_char.currentY = ground_obj.yPos - OBJECT_HEIGHT; // to get the offset of the mario image standing on ground
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