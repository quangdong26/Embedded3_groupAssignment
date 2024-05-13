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
    mario_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // to get the offset of the mario image standing on ground
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, marioImg, OBJECT_WIDTH, OBJECT_HEIGHT);
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
        mario_char.currentPos.X += delta_x;
        deleteImage(mario_char.pastPos.X, mario_char.currentPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
    }

    if (delta_y != 0) {
        mario_char.pastPos.Y = mario_char.currentPos.Y;
        mario_char.currentPos.Y += delta_y;
        deleteImage(mario_char.currentPos.X, mario_char.pastPos.Y, OBJECT_WIDTH, OBJECT_HEIGHT);
    }

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