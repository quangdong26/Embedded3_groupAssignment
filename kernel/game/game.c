#include "./game.h"

volatile int gameState = GAME_OFF;
volatile int isGameInit = DEFAULT;
int frameCounter = 0;

mario_t mario_char;
ground_t ground_obj;
obstacle_t mario_obstacle;

int w_pressed = 0;
int a_pressed = 0;
int s_pressed = 0;
int d_pressed = 0;

void initStatMario(void) {
    mario_char.height_size = OBJECT_HEIGHT;
    mario_char.width_size = OBJECT_WIDTH;
    mario_char.currentPos.X = ground_obj.groundPos.X;
    mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Mario starts on the ground
    mario_char.isJumping = 0;
    mario_char.jumpVelocity = 0;
    mario_char.horizontalSpeed = 0;
    mario_char.canDoubleJump = 1; // Allow double jump initially
    setHitBox(sizeof(mario_char));
}

void clearScreen(void) {
    deleteImage(DEFAULT, DEFAULT, 3000, 3000); // Clear the screen
}

void renderBackGround(void) {
    clearScreen(); // Clear the previous frame
    drawGround(); // Draw the ground
}

void drawGround(void) {
    ground_obj.groundPos.X = GND_X_POS;
    ground_obj.groundPos.Y = GND_Y_POS;
    ground_obj.width = WINDOW_WIDTH - ground_obj.groundPos.X;
    ground_obj.height = WINDOW_HEIGHT - ground_obj.groundPos.Y;
    drawArrayPixel(ground_obj.groundPos.X, ground_obj.groundPos.Y, 0xFFA500, ground_obj.width, ground_obj.height); // Draw ground
}

void drawObstacle(void) {
    mario_obstacle.obstaclePos.X = WINDOW_WIDTH - OBJECT_WIDTH;
    mario_obstacle.obstaclePos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT;
    uart_puts(" Obstacle coordinate: ");
    uart_dec(mario_obstacle.obstaclePos.Y);
    uart_puts("\n");
    drawArrayPixel(mario_obstacle.obstaclePos.X, mario_obstacle.obstaclePos.Y, 0x00FF00, OBJECT_WIDTH, OBJECT_HEIGHT); // Draw obstacle
}

void renderPlayerInitPoint(void) {
    mario_char.height_size = OBJECT_HEIGHT;
    mario_char.width_size = OBJECT_WIDTH;
    mario_char.currentPos.X = ground_obj.groundPos.X;
    mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Mario starts on the ground
    mario_char.isJumping = 0;
    mario_char.jumpVelocity = 0;
    mario_char.horizontalSpeed = 0;
    mario_char.canDoubleJump = 1; // Allow double jump initially
    setHitBox(sizeof(mario_char));
    uart_dec(mario_char.marioHitBox.bottom_right_corner.Y);
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT); // Display Mario
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

void handleJumping(void) {
    if (mario_char.isJumping) { // If Mario is jumping
        if (frameCounter % FRAME_DELAY == 0) {
            mario_char.pastPos.Y = mario_char.currentPos.Y;
            mario_char.currentPos.Y -= mario_char.jumpVelocity; // Move Mario up
            mario_char.jumpVelocity -= GRAVITY; // Reduce jump velocity by gravity

            mario_char.pastPos.X = mario_char.currentPos.X;
            mario_char.currentPos.X += mario_char.horizontalSpeed; // Apply horizontal movement

            if (mario_char.currentPos.Y >= ground_obj.groundPos.Y - OBJECT_HEIGHT) {
                mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Snap back to ground level
                mario_char.isJumping = 0; // End the jump
                mario_char.jumpVelocity = 0; // Reset jump velocity
                mario_char.horizontalSpeed = 0; // Reset horizontal speed
                mario_char.jumpDirection = 0; // Reset jump direction
                mario_char.canDoubleJump = 1; // Reset double jump
            }

            deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
            displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT); // Update Mario's position
            setHitBox(sizeof(mario_char)); // Update hitbox
        }
    }
}

void handleHorizontalMovement(MarioAction action) {
    float acceleration = 1.0; // Increased acceleration
    float maxSpeed = 15.0;
    float deceleration = 3.0; // Increased deceleration
    float threshold = 1.5; // Speed threshold for sharper turns

    switch (action) {
        case MOVE_RIGHT:
            if (mario_char.horizontalSpeed < -threshold) {
                mario_char.horizontalSpeed += deceleration; // Decelerate if moving left
            } else {
                mario_char.horizontalSpeed += acceleration; // Accelerate right
            }
            if (mario_char.horizontalSpeed > maxSpeed) mario_char.horizontalSpeed = maxSpeed; // Cap speed
            d_pressed = 1;
            break;
        case MOVE_LEFT:
            if (mario_char.horizontalSpeed > threshold) {
                mario_char.horizontalSpeed -= deceleration; // Decelerate if moving right
            } else {
                mario_char.horizontalSpeed -= acceleration; // Accelerate left
            }
            if (mario_char.horizontalSpeed < -maxSpeed) mario_char.horizontalSpeed = -maxSpeed; // Cap speed
            a_pressed = 1;
            break;
        default:
            if (mario_char.horizontalSpeed > 0) {
                mario_char.horizontalSpeed -= deceleration; // Decelerate to stop
                if (mario_char.horizontalSpeed < 0) mario_char.horizontalSpeed = 0;
            } else if (mario_char.horizontalSpeed < 0) {
                mario_char.horizontalSpeed += deceleration; // Decelerate to stop
                if (mario_char.horizontalSpeed > 0) mario_char.horizontalSpeed = 0;
            }
            a_pressed = 0;
            d_pressed = 0;
            break;
    }

    if (mario_char.horizontalSpeed != 0) { // Update position if moving
        mario_char.pastPos.X = mario_char.currentPos.X;
        mario_char.pastPos.Y = mario_char.currentPos.Y;
        mario_char.currentPos.X += mario_char.horizontalSpeed;

        deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
        displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
        setHitBox(sizeof(mario_char));
    }
}

void applyGravity(void) {
    if (!mario_char.isJumping && mario_char.currentPos.Y < ground_obj.groundPos.Y - OBJECT_HEIGHT) { // Apply gravity if not jumping
        if (frameCounter % FRAME_DELAY == 0) {
            mario_char.pastPos.Y = mario_char.currentPos.Y;
            mario_char.currentPos.Y += GRAVITY;

            mario_char.pastPos.X = mario_char.currentPos.X;
            mario_char.currentPos.X += mario_char.horizontalSpeed;

            if (mario_char.currentPos.Y > ground_obj.groundPos.Y - OBJECT_HEIGHT) {
                mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Snap back to ground level
            }

            deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
            displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
            setHitBox(sizeof(mario_char)); // Update hitbox
        }
    }
}

void marioMovement(MarioAction action) {
    if (action == JUMP || action == MOVE_RIGHT_JUMP || action == MOVE_LEFT_JUMP) { // Handle jumping
        if (!mario_char.isJumping && mario_char.currentPos.Y == ground_obj.groundPos.Y - OBJECT_HEIGHT) {
            mario_char.isJumping = 1;
            mario_char.jumpVelocity = JUMP_VELOCITY;

            if (action == MOVE_RIGHT_JUMP) {
                mario_char.jumpDirection = 1; // Jump to the right
                mario_char.horizontalSpeed = 5; // Adjust horizontal speed
            } else if (action == MOVE_LEFT_JUMP) {
                mario_char.jumpDirection = -1; // Jump to the left
                mario_char.horizontalSpeed = -5; // Adjust horizontal speed
            } else {
                mario_char.jumpDirection = 0; // Jump straight up
                mario_char.horizontalSpeed = 0; // No horizontal movement
            }
        } else if (mario_char.canDoubleJump && mario_char.isJumping) {
            mario_char.jumpVelocity = JUMP_VELOCITY; // Double jump
            mario_char.canDoubleJump = 0; // Disable further double jumps
        }
    } else {
        handleHorizontalMovement(action); // Handle horizontal movement
    }

    uart_puts("\nBottom right corner X: ");
    uart_dec(mario_char.marioHitBox.bottom_right_corner.X);
    uart_puts(" bottom right corner Y: ");
    uart_dec(mario_char.marioHitBox.bottom_right_corner.Y);
}

void reset(void) {
    renderBackGround(); // Render the background
    renderPlayerInitPoint(); // Render Mario's starting position
    drawObstacle(); // Draw the obstacle
}

int checkCollision(int obj1, int obj2) {
    // If the comparing objects are Mario and obstacle
    if (obj1 == sizeof(mario_char) && obj2 == sizeof(mario_obstacle)) {
        if ((mario_char.marioHitBox.bottom_right_corner.X >= mario_obstacle.obstaclePos.X &&
             mario_char.marioHitBox.bottom_right_corner.Y >= mario_obstacle.obstaclePos.Y + 20) ||
            (mario_char.marioHitBox.bottom_left_corner.X <= mario_obstacle.obstacleHitBox.top_right_corner.X &&
             mario_char.marioHitBox.bottom_left_corner.Y >= mario_obstacle.obstacleHitBox.top_right_corner.Y + 20)) {
            return 1; // Collision detected
        }
    }
    return 0; // No collision
}

void changeBoxSize(coordinate_t *dat, coordinate_t object_pos, int width_size, int height_size, int coord_pos) {
    if (coord_pos == TOP_LEFT_CORNER) {
        dat->X = object_pos.X + HITBOX_OFFSET;
        dat->Y = object_pos.Y + HITBOX_OFFSET;
    } else if (coord_pos == TOP_RIGHT_CORNER) {
        dat->X = (object_pos.X + width_size) - HITBOX_OFFSET;
        dat->Y = object_pos.Y + HITBOX_OFFSET;
    } else if (coord_pos == BOTTOM_LEFT_CORNER) {
        dat->X = object_pos.X + HITBOX_OFFSET;
        dat->Y = height_size - HITBOX_OFFSET;
    } else if (coord_pos == BOTTOM_RIGHT_CORNER) {
        dat->X = (object_pos.X + width_size) - HITBOX_OFFSET;
        dat->Y = height_size - HITBOX_OFFSET;
    }
}

void setHitBox(int objLen) {
    // Set hitbox for the testing object
    switch (objLen) {
        case sizeof(mario_char):
            changeBoxSize(&mario_char.marioHitBox.bottom_left_corner, mario_char.currentPos, OBJECT_WIDTH, OBJECT_HEIGHT, TOP_LEFT_CORNER);
            changeBoxSize(&mario_char.marioHitBox.bottom_right_corner, mario_char.currentPos, OBJECT_WIDTH, OBJECT_HEIGHT, TOP_RIGHT_CORNER);
            changeBoxSize(&mario_char.marioHitBox.top_left_corner, mario_char.currentPos, OBJECT_WIDTH, OBJECT_HEIGHT, BOTTOM_LEFT_CORNER);
            changeBoxSize(&mario_char.marioHitBox.top_right_corner, mario_char.currentPos, OBJECT_WIDTH, OBJECT_HEIGHT, BOTTOM_RIGHT_CORNER);
            break;
        case sizeof(mario_obstacle):
            changeBoxSize(&mario_obstacle.obstacleHitBox.bottom_left_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBJECT_HEIGHT, TOP_LEFT_CORNER);
            changeBoxSize(&mario_obstacle.obstacleHitBox.bottom_right_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBJECT_HEIGHT, TOP_RIGHT_CORNER);
            changeBoxSize(&mario_obstacle.obstacleHitBox.top_left_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBJECT_HEIGHT, BOTTOM_LEFT_CORNER);
            changeBoxSize(&mario_obstacle.obstacleHitBox.top_right_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBJECT_HEIGHT, BOTTOM_RIGHT_CORNER);
        default:
            break;
    }

    // for future use if we want to add more objects
    
}

void gameOn(void) {
    static int delayCounter = 0; // Add delay counter

    // Setting up the initial value for game
    if (isGameInit == DEFAULT) {
        renderBackGround();
        renderPlayerInitPoint();
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
    handleJumping();
    applyGravity();

    // Check for collisions
    if (checkCollision(sizeof(mario_char), sizeof(mario_obstacle))) {
        reset();
    }

    // Increment frame counter
    frameCounter++;
}