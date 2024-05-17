#include "./game.h"

volatile int gameState = GAME_OFF;
volatile int isGameInit = DEFAULT;

void clearScreen(void) {
    deleteImage(DEFAULT, DEFAULT, 3000, 3000); // Clear the screen
}

void renderBackGround(void) {
    clearScreen(); // Clear the previous frame
    drawGround(); // Draw the ground
}

void drawGround(void) {
    setGroundObject();
    drawArrayPixel(ground_obj.groundPos.X, ground_obj.groundPos.Y, 0xFFA500, ground_obj.width, ground_obj.height); // Draw ground
}

void drawObstacle(void) {
    
    setObStacleObject();
    uart_puts("\nObstacle coordinate X: ");
    uart_dec(mario_obstacle.obstacleHitBox.top_left_corner.X);
    uart_puts(" Obstacle coordinate Y: ");
    uart_dec(mario_obstacle.obstacleHitBox.bottom_left_corner.Y);
    uart_puts("\n");
    drawArrayPixel(mario_obstacle.obstaclePos.X,  mario_obstacle.obstaclePos.Y, 0x00FF00, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
}

void drawMario(void) {
    renderPlayerInitPoint();
    uart_dec(mario_char.marioHitBox.bottom_right_corner.Y);
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
    //drawArrayPixel(mario_char.marioHitBox.top_left_corner.X, mario_char.marioHitBox.top_left_corner.Y, 0x00FF00, OBJECT_WIDTH, OBJECT_HEIGHT);
}

void updateObject(int objLen, int offsetX, int offsetY) {
    if (objLen == sizeof(mario_char)) { // Check if the object is Mario
        mario_char.currentPos.X += offsetX;
        mario_char.currentPos.Y += offsetY;
        mario_char.marioHitBox.top_left_corner.X += offsetX;
        mario_char.marioHitBox.top_right_corner.X += offsetX;
        mario_char.marioHitBox.bottom_left_corner.X += offsetX;
        mario_char.marioHitBox.bottom_right_corner.X += offsetX;
        mario_char.marioHitBox.top_left_corner.Y += offsetY;
        mario_char.marioHitBox.top_right_corner.Y += offsetY;
        mario_char.marioHitBox.bottom_left_corner.Y += offsetY;
        mario_char.marioHitBox.bottom_right_corner.Y += offsetY;
    }
}

void reset(void) {
    renderBackGround(); // Render the background
    drawMario();
    drawObstacle(); // Draw the obstacle
}

/**
 * @brief check if the current object is on the right or left of the mario
*/
int updateAbsolutePosition(int objLen) {
    switch (objLen)
    {
    case sizeof(mario_obstacle):
        if(mario_char.marioHitBox.bottom_right_corner.X <= mario_obstacle.obstacleHitBox.top_left_corner.X) {
            mario_obstacle.isRightToMario = 1;
        }
        return mario_obstacle.isRightToMario;
        break;
    
    default:
        break;
    }
}


/** @brief this function check if 2 boject collision or not
 * It will check 
 * mario_char.marioHitBox.bottom_right_corner.Y >= mario_obstacle.obstaclePos.Y + 20 ||
           mario_char.marioHitBox.bottom_left_corner.X <= mario_obstacle.obstacleHitBox.top_right_corner.X &&
           mario_char.marioHitBox.bottom_left_corner.Y >= mario_obstacle.obstacleHitBox.top_right_corner.Y + 20
*/
int checkCollision(int obj1, int obj2) {
    //if the comparing object are mario and obstacle
    if(obj1 == sizeof(mario_char) && obj2 == sizeof(mario_obstacle)) {
        updateAbsolutePosition(sizeof(mario_obstacle)); // check if the obstacle on the right or left of the mario
        if(mario_char.marioHitBox.bottom_right_corner.X >= mario_obstacle.obstacleHitBox.top_left_corner.X &&
           mario_char.marioHitBox.bottom_right_corner.Y <= mario_obstacle.obstacleHitBox.top_left_corner.Y && mario_obstacle.isRightToMario ||
           mario_char.marioHitBox.bottom_left_corner.X <= mario_obstacle.obstacleHitBox.top_right_corner.X &&
           mario_char.marioHitBox.bottom_left_corner.Y <= mario_obstacle.obstacleHitBox.top_right_corner.Y && !mario_obstacle.isRightToMario) { // check colision mario
            return 1;
        }
    }
    return 0; // No collision
}



void gameOn(void) {
    static int delayCounter = 0; // Add delay counter

    // Setting up the initial value for game
    if (isGameInit == DEFAULT) {
        renderBackGround();
        drawMario();
        drawObstacle();
        isGameInit = INIT;
    }

    // Handle input
    if (delayCounter > 0) {
        delayCounter--; // Decrement delay counter
    } else if (uart_has_data()) { // Check if there's input to read
        char c = uart_getc();
        switch (c) {
            case 'w':
                if (d_pressed) {
                    marioMovement(MOVE_RIGHT_JUMP);
                } else if (a_pressed) {
                    marioMovement(MOVE_LEFT_JUMP);
                } else {
                    marioMovement(JUMP);
                }
                w_pressed = 1;
                break;
            case 'a':
                if (!w_pressed) {
                    marioMovement(MOVE_LEFT);
                } else {
                    marioMovement(MOVE_LEFT_JUMP);
                }
                a_pressed = 1;
                break;
            case 'd':
                if (!w_pressed) {
                    marioMovement(MOVE_RIGHT);
                } else {
                    marioMovement(MOVE_RIGHT_JUMP);
                }
                d_pressed = 1;
                break;
            case 's':
                marioMovement(CROUCH);
                break;
            case 'r':
                reset();
                break;
            default:
                break;
        }

        // Add delay to prevent rapid key presses
        if (c != 'w') {
            w_pressed = 0;
        }
        if (c != 'a') {
            a_pressed = 0;
        }
        if (c != 'd') {
            d_pressed = 0;
        }
    }

    // Apply gravity and handle jumping every cycle
    handleJumping(); // belong to mario object
    applyGravity(); // belong to mario object

    //handle the scene scroller
    handleSceneTransition();

    // Check for collisions
    if (checkCollision(sizeof(mario_char), sizeof(mario_obstacle))) {
        reset();
    }

    // Increment frame counter
    frameCounter++;
}

void moveObstacleToLeft(void) {
    deleteImage(mario_obstacle.obstaclePos.X, mario_obstacle.obstaclePos.Y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
    mario_obstacle.obstaclePos.X = mario_obstacle.obstaclePos.X - TRANSITION_OFF;
    setObstacleHitBox(); // set the hitbox again
    drawArrayPixel(mario_obstacle.obstaclePos.X,  mario_obstacle.obstaclePos.Y, 0x00FF00, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
}


void handleSceneTransition(void) {
    if (mario_char.currentPos.X > SCENE_TRANSITION_X) {
        isReachTransition = 1;
        mario_char.currentPos.X = SCENE_TRANSITION_X;  // stick the mario position to the define pos  
        setMarioHitBox(); // define new hitbox
        moveObstacleToLeft(); // move the asset to the left
    } else {
        isReachTransition = 0;
    }
}