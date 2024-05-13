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

void marioMovement(MarioAction action) {
    int delta_x = 0;
    int delta_y = 0;

    // Determine the change in position based on the action
    switch (action) {
        case MOVE_RIGHT:
            delta_x = 10; 
            break;
        case MOVE_LEFT:
            delta_x = -10; 
            break;
        case JUMP:
            delta_y = -10; 
            break;
        case CROUCH:
            // Crouching does not change the x or y coordinates,
            // it could change Mario's height or trigger a sprite change
            // so I will continue this when we got the animation ready
            break;
    }

    // Update Mario's position based on the action
    if (delta_x != 0) {
        mario_char.pastX = mario_char.currentX;
        mario_char.currentX += delta_x;
        deleteImage(mario_char.pastX, mario_char.currentY, OBJECT_WIDTH, OBJECT_HEIGHT);
    }

    if (delta_y != 0) {
        mario_char.pastY = mario_char.currentY;
        mario_char.currentY += delta_y;
        deleteImage(mario_char.currentX, mario_char.pastY, OBJECT_WIDTH, OBJECT_HEIGHT);
    }

    // Display Mario's updated position
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

    switch (c) {
    case 'w': // Jump
        marioMovement(JUMP);
        break;
    case 'a': // Move left
        marioMovement(MOVE_LEFT);
        break;
    case 's': // Crouch
        marioMovement(CROUCH);
        break;
    case 'd': // Move right
        marioMovement(MOVE_RIGHT);
        break;
    case 'r': // Reset
        reset();
        break;
    default:
        break;
    }
}