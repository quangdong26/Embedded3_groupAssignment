#include "./game.h"
volatile int gameState = GAME_OFF;
volatile int isGameInit = DEFAULT;
mario_t mario_char;
ground_t ground_obj;
obstacle_t mario_obstacle;

void clearScreen(void) {
    deleteImage(DEFAULT, DEFAULT, 3000, 3000);
}
void renderBackGround(void) {
    clearScreen();
    // displayImage(0, 0, ground, 100, 100);
    drawGround();
}

void drawGround(void) {
    ground_obj.groundPos.X = GND_X_POS;
    ground_obj.groundPos.Y = GND_Y_POS;
    ground_obj.width = WINDOW_WIDTH - ground_obj.groundPos.X;
    ground_obj.height = WINDOW_HEIGHT -  ground_obj.groundPos.Y;

    drawArrayPixel(ground_obj.groundPos.X, ground_obj.groundPos.Y, 0xFFA500, ground_obj.width, ground_obj.height);
}

void drawObstacle(void) {
    mario_obstacle.obstaclePos.X = WINDOW_WIDTH - OBJECT_WIDTH;
    mario_obstacle.obstaclePos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT;
    uart_puts(" Obstacle coordinate: ");
    uart_dec(mario_obstacle.obstaclePos.Y);
    uart_puts("\n");
    drawArrayPixel(mario_obstacle.obstaclePos.X,  mario_obstacle.obstaclePos.Y, 0x00FF00, OBJECT_WIDTH, OBJECT_HEIGHT);
}

void renderPlayerInitPoint(void) {
    mario_char.height_size = OBJECT_HEIGHT;
    mario_char.width_size = OBJECT_WIDTH;
    mario_char.currentPos.X = ground_obj.groundPos.X;
    mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // to get the offset of the mario image standing on ground
    setHitBox(sizeof(mario_char));
    uart_puts("Bottom right corner X: ");
    uart_dec(mario_char.marioHitBox.bottom_right_corner.X);
    uart_puts(" bottom right corner Y: ");
    uart_dec(mario_char.marioHitBox.bottom_right_corner.Y);
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
}


void updateObject(int objLen, int offset_val, MarioAction condition) {
    if(objLen == sizeof(mario_char)) { // check if the object is mario
        if(condition == MOVE_RIGHT || condition == MOVE_LEFT) { // check if the object moving horizontally
            mario_char.currentPos.X += offset_val;
            mario_char.marioHitBox.top_left_corner.X += offset_val;
            mario_char.marioHitBox.top_right_corner.X += offset_val;
            mario_char.marioHitBox.bottom_left_corner.X += offset_val;
            mario_char.marioHitBox.bottom_right_corner.X += offset_val;
        } 
         else if(condition == JUMP) {
            mario_char.currentPos.Y += offset_val;
            mario_char.marioHitBox.top_left_corner.Y += offset_val;
            mario_char.marioHitBox.top_right_corner.Y += offset_val;
            mario_char.marioHitBox.bottom_left_corner.Y += offset_val;
            mario_char.marioHitBox.bottom_right_corner.Y += offset_val;
        }
    }
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
        mario_char.pastPos.X = mario_char.currentPos.X;
        updateObject(sizeof(mario_char), delta_x, action);
        deleteImage(mario_char.pastPos.X, mario_char.currentPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
    }

    if (delta_y != 0) {
        mario_char.pastPos.Y = mario_char.currentPos.Y;
        updateObject(sizeof(mario_char), delta_y, action);
        deleteImage(mario_char.currentPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
    }

    uart_puts("\nBottom right corner X: ");
    uart_dec(mario_char.marioHitBox.bottom_right_corner.X);
    uart_puts(" bottom right corner Y: ");
    uart_dec(mario_char.marioHitBox.bottom_right_corner.Y);

    // Display Mario's updated position
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
}

void reset(void) {
    renderBackGround(); 
    renderPlayerInitPoint();
    drawObstacle();
}

/** @brief this function check if 2 boject collision or not
 * It will check 
*/
int checkCollision(int obj1, int obj2) {
    //if the comparing object are mario and obstacle
    if(obj1 == sizeof(mario_char) && obj2 == sizeof(mario_obstacle)) {
        if(mario_char.marioHitBox.bottom_right_corner.X >= mario_obstacle.obstaclePos.X &&
           mario_char.marioHitBox.bottom_right_corner.Y >= mario_obstacle.obstaclePos.Y + 20 ||
           mario_char.marioHitBox.bottom_left_corner.X <= mario_obstacle.obstacleHitBox.top_right_corner.X &&
           mario_char.marioHitBox.bottom_left_corner.Y >= mario_obstacle.obstacleHitBox.top_right_corner.Y + 20) { // check colision mario
            return 1;
        }
    }
    return 0;
}

void changeBoxSize(coordinate_t *dat, int width_size, int height_size, int isLeft, int isTop) {
    if(isLeft == LEFT_CORNER) {
        dat->X = width_size + HITBOX_OFFSET;
        
    } else {
        dat->X = width_size - HITBOX_OFFSET;
    }

    if(isTop == TOP_CORNER) {
        dat->Y = height_size + HITBOX_OFFSET;
    } else {
        dat->Y = ground_obj.groundPos.Y - 20; // stick the bottom corner to ground
    }
}

/**
 * @brief this function used to set hitbox for every component in the game
*/

void setHitBox(int objLen) {
    switch (objLen)
    {
    case sizeof(mario_char):
        /* code */
        changeBoxSize(&mario_char.marioHitBox.bottom_left_corner, mario_char.width_size, mario_char.height_size, LEFT_CORNER, BOT_CORNER);
        changeBoxSize(&mario_char.marioHitBox.bottom_right_corner, mario_char.width_size, mario_char.height_size, RIGHT_CORNER, BOT_CORNER);
        changeBoxSize(&mario_char.marioHitBox.top_left_corner, mario_char.width_size, mario_char.height_size, LEFT_CORNER, TOP_CORNER);
        changeBoxSize(&mario_char.marioHitBox.top_right_corner, mario_char.width_size, mario_char.height_size, RIGHT_CORNER, TOP_CORNER);
        break;
    case sizeof(mario_obstacle):
        changeBoxSize(&mario_obstacle.obstacleHitBox.bottom_left_corner, mario_obstacle.width, mario_obstacle.height, LEFT_CORNER, BOT_CORNER);
        changeBoxSize(&mario_obstacle.obstacleHitBox.bottom_right_corner, mario_obstacle.width, mario_obstacle.height, RIGHT_CORNER, BOT_CORNER);
        changeBoxSize(&mario_obstacle.obstacleHitBox.top_left_corner, mario_obstacle.width, mario_obstacle.height, LEFT_CORNER, TOP_CORNER);
        changeBoxSize(&mario_obstacle.obstacleHitBox.top_right_corner, mario_obstacle.width, mario_obstacle.height, RIGHT_CORNER, TOP_CORNER);
    default:
        break;
    }
    // set hitbox for other assets 
}

void gameOn(void) {
    char c = uart_getc();
    // setting up the initial value for game
    if(isGameInit == DEFAULT) {
        renderBackGround(); 
        renderPlayerInitPoint();
        drawObstacle(); // draw the obstacle
        isGameInit = INIT;
    }

    if(checkCollision(sizeof(mario_char), sizeof(mario_obstacle))) {
        reset();
    }

    switch (c)
    {
    case 'w':
        /* code */
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