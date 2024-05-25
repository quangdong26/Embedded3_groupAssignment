#include "./game.h"
#include "../image/defaultMario.h"
#include "../image/mariofw.h"
#include "../image/terrian1.h"
#include "../image/terrian2.h"
#include "../image/terrian3.h"

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
    displayObject(ground_obj.groundPos.X, ground_obj.groundPos.Y, terrian1_terrian1, 480, 64); // Draw ground
    displayObject(ground_obj.groundPos.X + 480, ground_obj.groundPos.Y, terrian1_terrian1, 480, 64); // Draw ground
    displayObject(ground_obj.groundPos.X + 480 + 480, ground_obj.groundPos.Y - 90, terrian2_terrian2, 480, TERRIAN2_HEIGHT);
}

void drawObstacle(void) {
    
    setObStacleObject();
    drawArrayPixel(mario_obstacle.obstaclePos.X,  mario_obstacle.obstaclePos.Y, 0x00FF00, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
}

void drawMario(void) {
    renderPlayerInitPoint();
    int bottom_right_y = mario_char.marioHitBox.top_left_corner.Y + mario_char.marioHitBox.height;
    uart_dec(bottom_right_y); 
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, default_mario, OBJECT_WIDTH, OBJECT_HEIGHT);
    //drawArrayPixel(mario_char.marioHitBox.top_left_corner.X, mario_char.marioHitBox.top_left_corner.Y, 0x00FF00, OBJECT_WIDTH, OBJECT_HEIGHT);
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
    int bot_right_hoz = mario_char.marioHitBox.top_left_corner.X + mario_char.marioHitBox.width;
    switch (objLen)
    {
    case sizeof(mario_obstacle):
        if(bot_right_hoz <= mario_obstacle.obstacleHitBox.top_left_corner.X) {
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
int checkCollision(hitbox_t marioHitbox, hitbox_t obstacleHitbox) {
    // Check if Mario's hitbox and the obstacle's hitbox overlap
    int marioLeft = mario_char.marioHitBox.top_left_corner.X;
    int marioRight = mario_char.marioHitBox.top_left_corner.X; + marioHitbox.width;
    int marioTop = mario_char.marioHitBox.top_left_corner.Y;
    int marioBottom = mario_char.marioHitBox.top_left_corner.Y + marioHitbox.height;

    int obstacleLeft = mario_obstacle.obstacleHitBox.top_left_corner.X;
    int obstacleRight = mario_obstacle.obstacleHitBox.top_left_corner.X + obstacleHitbox.width;
    int obstacleTop = mario_obstacle.obstacleHitBox.top_left_corner.Y;
    int obstacleBottom = mario_obstacle.obstacleHitBox.top_left_corner.Y + obstacleHitbox.height;

    // Collision conditions
    int collisionX = (marioRight >= obstacleLeft) && (marioLeft <= obstacleRight);
    int collisionY = (marioBottom >= obstacleTop) && (marioTop <= obstacleBottom);

    return collisionX && collisionY; // If both true, there's a collision
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
                }
                a_pressed = 1;
                break;
            case 'd':
                if (!w_pressed) {
                    marioMovement(MOVE_RIGHT);
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
    if (checkCollision(mario_char.marioHitBox, mario_obstacle.obstacleHitBox)) {
        // mario_char.marioHitBox.bottom_left_corner.Y = mario_obstacle.obstacleHitBox.top_right_corner.Y;
        // mario_char.currentPos.Y = mario_char.marioHitBox.bottom_left_corner.Y - HITBOX_OFFSET;
        //reset();
    }

    // Increment frame counter
    frameCounter++;
}

// Function to find the y-coordinate of the line of pixels equal to 0x00 and just above 0x00ffcec6
int findLineAboveColor(unsigned long image[480][160], int width, int height, unsigned long color, unsigned long target) {
    // Traverse the image from the second row to the last row
    for (int j = 1; j < height; j++) {
        // Check each pixel in the current row
        for (int i = 0; i < width; i++) {
            // Check if the current pixel is the target color and the pixel above is the specified color
            if (image[j][i] == target && image[j-1][i] == color) {
                return j - 1;
            }
        }
    }
    // Return -1 if the pattern is not found
    return -1;
}

void moveObstacleToLeft(void) {
    // Clear the previous obstacle and ground frames
    deleteImage(mario_obstacle.obstaclePos.X, mario_obstacle.obstaclePos.Y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X, ground_obj.groundPos.Y, terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + TERRIAN1_WIDTH, ground_obj.groundPos.Y, terrian1_terrian1,TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + TERRIAN1_WIDTH + TERRIAN1_WIDTH, ground_obj.groundPos.Y - 90, terrian2_terrian2,TERRIAN2_WIDTH, TERRIAN2_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + TERRIAN1_WIDTH + TERRIAN1_WIDTH + TERRIAN1_WIDTH, ground_obj.groundPos.Y - 90, terrian3_terrian3,TERRIAN3_WIDTH, TERRIAN3_HEIGHT);


//    if ((mario_obstacle.obstaclePos.X - TRANSITION_OFF >= 0) && (ground_obj.groundPos.X - TRANSITION_OFF >= -480)) {
        // Move obstacle
        mario_obstacle.obstaclePos.X -= TRANSITION_OFF;
        setObstacleHitBox(); // Reset the hitbox
        drawArrayPixel(mario_obstacle.obstaclePos.X, mario_obstacle.obstaclePos.Y, 0x00FF00, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);
        ground_obj.groundPos.X -= TRANSITION_OFF;
        displayObject(ground_obj.groundPos.X, ground_obj.groundPos.Y, terrian1_terrian1,TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
        displayObject(ground_obj.groundPos.X + TERRIAN1_WIDTH, ground_obj.groundPos.Y, terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
        displayObject(ground_obj.groundPos.X + TERRIAN1_WIDTH + TERRIAN1_WIDTH,ground_obj.groundPos.Y - 90, terrian2_terrian2,TERRIAN2_WIDTH, TERRIAN2_HEIGHT);
        displayObject(ground_obj.groundPos.X + TERRIAN1_WIDTH + TERRIAN1_WIDTH + TERRIAN1_WIDTH, ground_obj.groundPos.Y - 90, terrian3_terrian3, TERRIAN3_WIDTH, TERRIAN3_HEIGHT);
    // } else {
    //     //displayObject(ground_obj.groundPos.X, ground_obj.groundPos.Y, terrian2_terrian2, 480, 64);
    //             // Move the ground
    //     ground_obj.groundPos.X -= TRANSITION_OFF;
    //     int new_ground_x = GND_X_POS;
    //     int new_ground_y = GND_Y_POS;
    //     int new_terr[480][160];
    //     convert1DTo2D(terrian2_terrian2, TERRIAN2_WIDTH, TERRIAN2_HEIGHT, new_terr);
    //     displayObject(new_ground_x, new_ground_y - 90, terrian2_terrian2, TERRIAN2_WIDTH, TERRIAN2_HEIGHT);
    //     displayObject(new_ground_x + TERRIAN2_WIDTH, new_ground_y - 90, terrian3_terrian3, TERRIAN3_WIDTH, TERRIAN3_HEIGHT);
    //     displayObject(new_ground_x + TERRIAN2_WIDTH + TERRIAN2_WIDTH, new_ground_y - 90, terrian2_terrian2, TERRIAN2_WIDTH, TERRIAN2_HEIGHT);
    // }
}



void handleSceneTransition(void) {
    if (mario_char.currentPos.X > SCENE_TRANSITION_X) {
        isReachTransition = 1;
        mario_char.currentPos.X = SCENE_TRANSITION_X;
        setMarioHitBox();
        moveObstacleToLeft(); 
    } else { // This 'else' handles BOTH scenarios
        if (mario_char.currentPos.X < INITIAL_POSITION_X) {
            mario_char.currentPos.X = INITIAL_POSITION_X;
        }
        isReachTransition = 0; // Reset only if Mario isn't transitioning
    }
}