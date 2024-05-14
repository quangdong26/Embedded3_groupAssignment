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
    uart_puts("Obstacle coordinate: ");
    uart_dec(mario_obstacle.obstaclePos.X);
    uart_puts("\n");
    drawArrayPixel(mario_obstacle.obstaclePos.X,  mario_obstacle.obstaclePos.Y, 0x00FF00, OBJECT_WIDTH, OBJECT_HEIGHT);
}

void renderPlayerInitPoint(void) {
    mario_char.height_size = OBJECT_HEIGHT;
    mario_char.width_size = OBJECT_WIDTH;
    mario_char.currentPos.X = ground_obj.groundPos.X;
    mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT;
    mario_char.isJumping = 0;
    mario_char.jumpVelocity = 0;
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
}

void applyGravity(void) {
    if (!mario_char.isJumping && mario_char.currentPos.Y < ground_obj.groundPos.Y - OBJECT_HEIGHT) {
        mario_char.pastPos.Y = mario_char.currentPos.Y;
        mario_char.currentPos.Y += GRAVITY;
        if (mario_char.currentPos.Y > ground_obj.groundPos.Y - OBJECT_HEIGHT) {
            mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Snap back to ground level
            mario_char.isJumping = 0; // Stop jumping
        }
        deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
        displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
    }
}

void marioMovement(MarioAction action) {
    int delta_x = 0;

    // Handle horizontal movement
    switch (action) {
        case MOVE_RIGHT:
            delta_x = 10; 
            break;
        case MOVE_LEFT:
            delta_x = -10; 
            break;
        case JUMP:
            // Only allow jumping if Mario is on the ground and not already jumping
            if (!mario_char.isJumping && mario_char.currentPos.Y == ground_obj.groundPos.Y - OBJECT_HEIGHT) {
                mario_char.isJumping = 1;
                mario_char.jumpVelocity = JUMP_VELOCITY;
            }
            break;
        case CROUCH:
            // Crouch action implement later
            break;
    }

    // Update horizontal position
    if (delta_x != 0) {
        mario_char.pastPos.X = mario_char.currentPos.X;
        mario_char.currentPos.X += delta_x;
        deleteImage(mario_char.pastPos.X, mario_char.currentPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
        displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
    }

    // Handle vertical movement if Mario is jumping
    if (mario_char.isJumping) {
        mario_char.pastPos.Y = mario_char.currentPos.Y;
        mario_char.currentPos.Y -= mario_char.jumpVelocity; // Move Mario up
        mario_char.jumpVelocity -= GRAVITY; // Reduce the jump velocity by gravity

        if (mario_char.currentPos.Y >= ground_obj.groundPos.Y - OBJECT_HEIGHT) {
            mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Snap back to ground level
            mario_char.isJumping = 0; // End the jump
            mario_char.jumpVelocity = 0; // Reset jump velocity
        }

        deleteImage(mario_char.pastPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
        displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
    }
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
        if(mario_char.currentPos.X + mario_char.width_size >= mario_obstacle.obstaclePos.X) {
            return 1;
        }
    }
    return 0;
}

void *my_memcpy(void *dest, const void *src, size_t n) {
    // Create pointers to work with byte-level manipulation
    char *dst = (char *)dest;
    const char *source = (const char *)src;

    // Copy each byte from source to destination
    for (size_t i = 0; i < n; i++) {
        dst[i] = source[i];
    }

    // Return the destination pointer
    return dest;
}

void gameOn(char c) {
    // Handle initialization
    if (isGameInit == DEFAULT) {
        renderBackGround(); 
        renderPlayerInitPoint();
        drawObstacle(); 
        isGameInit = INIT;
    }

    // Apply gravity every cycle
    applyGravity();

    // Check for collisions
    if (checkCollision(sizeof(mario_char), sizeof(mario_obstacle))) {
        reset();
    }

    // Handle input
    switch (c) {
        case 'w':
            marioMovement(JUMP);
            break;
        case 'a':
            marioMovement(MOVE_LEFT);
            break;
        case 's':
            marioMovement(CROUCH);
            break;
        case 'd':
            marioMovement(MOVE_RIGHT);
            break;
        case 'r':
            reset();
            break;
        default:
            break;
    }
}