#include "./game.h"

volatile int gameState = GAME_OFF;
volatile int isGameInit = DEFAULT;

volatile int isHitObstacle = 0;


void clearScreen(void) {
    deleteImage(DEFAULT, DEFAULT, 3000, 3000); // Clear the screen
}

void renderBackGround(void) {
    clearScreen(); // Clear the previous frame
    drawGround(); // Draw the ground
}

void drawGround(void) {
    int ground_width = PHYSICAL_WINDOW_WIDTH - ground_obj.groundPos.X;
    int ground_height = PHYSICAL_WINDOW_HEIGHT - ground_obj.groundPos.Y;
    setGroundObject(&ground_obj, GND_X_POS, GND_Y_POS, ground_width, ground_height); // create base ground object instance of

    displayObject(ground_obj.groundPos.X, ground_obj.groundPos.Y, terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT); // Draw ground
    displayObject(ground_obj.groundPos.X + GND_LENGTH, ground_obj.groundPos.Y, terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT); // Draw ground

    // set ground instance for terrian 2:
    setGroundObject(&terrian2, ground_obj.groundPos.X + 2 * GND_LENGTH, GND_Y_POS - 90, TERRIAN2_WIDTH, TERRIAN2_HEIGHT); // the terrian2 is 90 before the base ground
    displayObject(terrian2.groundPos.X, terrian2.groundPos.Y, terrian2_terrian2, TERRIAN2_WIDTH, TERRIAN2_HEIGHT);

    // set ground instance for terrian 3:
    setGroundObject(&terrian3, ground_obj.groundPos.X + 3 * GND_LENGTH, GND_Y_POS - 90, TERRIAN3_WIDTH, TERRIAN3_HEIGHT); // the terrian2 is 90 before the base ground
    displayObject(terrian3.groundPos.X, terrian3.groundPos.Y, terrian3_terrian3, TERRIAN3_WIDTH, TERRIAN3_HEIGHT);
}

void drawMario(void) {
    renderPlayerInitPoint();
    int bottom_right_y = mario_char.marioHitBox.top_left_corner.Y + mario_char.marioHitBox.height;
    displayObject(mario_char.currentPos.X, mario_char.currentPos.Y, default_mario, OBJECT_WIDTH, OBJECT_HEIGHT);
    //drawArrayPixel(mario_char.marioHitBox.top_left_corner.X, mario_char.marioHitBox.top_left_corner.Y, 0x00FF00, OBJECT_WIDTH, OBJECT_HEIGHT);
}


void drawGoomba(void) {
    renderGoombaInitPoint();
    // int bottom_right_y = goomba_char.goombaHitBox.top_left_corner.Y + goomba_char.goombaHitBox.height;
    displayObject(goomba_char.currentPos.X, goomba_char.currentPos.Y, defaultGoomba, OBJECT_WIDTH, OBJECT_HEIGHT);
}

/**
 * @brief define all the instances of the terrian object
 * @details This function should be called in the initialization of the game
*/
void defineObstacles(void) {
    setObStacleObject(&terrian2_obstacle, terrian2.groundPos.X + TERRIAN2_OBSTACLE_X_OFFSET, terrian2.groundPos.Y + TERRIAN2_OBSTACLE_Y_OFFSET, OBJECT_WIDTH, OBJECT_HEIGHT);
    setObStacleObject(&terrian2_stair, terrian2.groundPos.X + TERRIAN2_STAIR_X_OFFSET, terrian2.groundPos.Y + TERRIAN2_STAIR_Y_OFFSET, STAIR_WIDTH, STAIR_HEIGHT);
    setObStacleObject(&terrian3_stair, terrian3.groundPos.X + TERRIAN3_STAIR_X_OFFSET, terrian2.groundPos.Y + TERRIAN3_STAIR_Y_OFFSET, STAIR_WIDTH, STAIR_HEIGHT);
}


void reset(void) {
    renderBackGround(); // Render the background
    drawMario();
    defineObstacles(); // after reset, define obstacle again
    isFallingHole = 0;
}



/** @brief this function check if 2 boject collision or not
 * It will check 
 * mario_char.marioHitBox.bottom_right_corner.Y >= mario_obstacle.obstaclePos.Y + 20 ||
           mario_char.marioHitBox.bottom_left_corner.X <= mario_obstacle.obstacleHitBox.top_right_corner.X &&
           mario_char.marioHitBox.bottom_left_corner.Y >= mario_obstacle.obstacleHitBox.top_right_corner.Y + 20
*/
int checkCollisionObstacle(mario_t tmp_char, obstacle_t des_obstacle) {
    // Check if Mario's hitbox and the obstacle's hitbox overlap
    int marioLeft = tmp_char.marioHitBox.top_left_corner.X;
    int marioRight = tmp_char.marioHitBox.top_left_corner.X + tmp_char.marioHitBox.width;
    int marioTop = tmp_char.marioHitBox.top_left_corner.Y;
    int marioBottom = tmp_char.marioHitBox.top_left_corner.Y + tmp_char.marioHitBox.height;

    int obstacleLeft = des_obstacle.obstacleHitBox.top_left_corner.X;
    int obstacleRight = des_obstacle.obstacleHitBox.top_left_corner.X + des_obstacle.width;
    int obstacleTop = des_obstacle.obstacleHitBox.top_left_corner.Y;
    int obstacleBottom = des_obstacle.obstacleHitBox.top_left_corner.Y + des_obstacle.height;


    // Collision conditions
    int collisionX = (marioRight >= obstacleLeft) && (marioLeft <= obstacleRight);
    int collisionY = (marioBottom >= obstacleTop) && (marioTop <= obstacleBottom);

    return collisionX && collisionY; // If both true, there's a collision
}

/**
 * @brief to check if the mario is entering the valley zone or not
*/
int checkEnterValley(mario_t tmp_char) {
    return tmp_char.marioHitBox.top_left_corner.X + tmp_char.marioHitBox.width >= terrian3.groundPos.X + TERRIAN3_VALLEY_X &&
            tmp_char.marioHitBox.top_left_corner.X + tmp_char.marioHitBox.width <= terrian3.groundPos.X + TERRIAN3_STAIR_X_OFFSET &&
            tmp_char.marioHitBox.top_left_corner.Y  < terrian3.groundPos.Y + TERRIAN3_VALLEY_Y - OBJECT_HEIGHT;
}

void gameOn(void) {
    static int delayCounter = 0; // Add delay counter
    // Setting up the initial value for game
    if (isGameInit == DEFAULT) {
        //renderBackGround(); // LEVEL 1
        renderBackGround_LV2(); //LEVEL 2
        defineObstacles();
        drawMario(); 
        drawGoomba();
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

    if (checkCollisionObstacle(mario_char, terrian2_stair) || checkCollisionObstacle(mario_char, terrian2_obstacle) || checkCollisionObstacle(mario_char, terrian3_stair)) {
        isHitObstacle = 1;
    } else {
        isHitObstacle = 0; // to set the flag to freeze mario
    }

    // if (isEnterValley && mario_char.marioHitBox.top_left_corner.X + OBJECT_WIDTH >= terrian3.groundPos.X + 170) {
    //     reset();
    //     //isHitObstacle = 1;
    // }

    if(checkEnterValley(mario_char)) {
        isEnterValley = 1;
        //reset();
    } else {
        isEnterValley = 0;
    }

    if(isFallingHole) {
        reset();
    }

    // Increment frame counter
    frameCounter++;
}

/**
 * @brief to display the terrian after updating
*/
void ground_transition_handle(void) {
    displayObject(ground_obj.groundPos.X, ground_obj.groundPos.Y, terrian1_terrian1,TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    displayObject(ground_obj.groundPos.X + TERRIAN1_WIDTH, ground_obj.groundPos.Y, terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    displayObject(terrian2.groundPos.X, terrian2.groundPos.Y, terrian2_terrian2, TERRIAN2_WIDTH, TERRIAN2_HEIGHT);
    displayObject(ground_obj.groundPos.X + TERRIAN1_WIDTH + TERRIAN1_WIDTH + TERRIAN1_WIDTH, ground_obj.groundPos.Y - 90, terrian3_terrian3, TERRIAN3_WIDTH, TERRIAN3_HEIGHT);
}

/**
 * @brief to update the terrian based on base ground
*/
void update_terrian_base(void) {
    ground_obj.groundPos.X -= TRANSITION_OFF;
    setGroundObject(&terrian2, ground_obj.groundPos.X + 2 * GND_LENGTH, GND_Y_POS - 90, TERRIAN2_WIDTH, TERRIAN2_HEIGHT); // update terrian 2 based on terrian 1
    setGroundObject(&terrian3, ground_obj.groundPos.X + 3 * GND_LENGTH, GND_Y_POS - 90, TERRIAN3_WIDTH, TERRIAN3_HEIGHT); // update terrian 3 based on terrian 1
    defineObstacles(); //redefine obstacle based on the terrian 2 
}

void moveObstacleToLeft(void) {
    deleteAnimationFrame(ground_obj.groundPos.X, ground_obj.groundPos.Y, terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + TERRIAN1_WIDTH, ground_obj.groundPos.Y, terrian1_terrian1,TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    deleteAnimationFrame(terrian2.groundPos.X, terrian2.groundPos.Y, terrian2_terrian2,TERRIAN2_WIDTH, TERRIAN2_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + TERRIAN1_WIDTH + TERRIAN1_WIDTH + TERRIAN1_WIDTH, ground_obj.groundPos.Y - 90, terrian3_terrian3,TERRIAN3_WIDTH, TERRIAN3_HEIGHT);


    // handle ground transition
    update_terrian_base();
    ground_transition_handle();
    
}
void lv2 () {
    moveObstacleToLeft_LV2();
}

const int ground2Y[6] = {-181,-180, -229, -180, -183,-22}; // use this array to modify the Y dimension of each terrian
const int ground2X[6] = {0, 480, 960, 1440, 1920, 2400}; // use this array to modify the X dimension of each terrian

void renderBackGround_LV2(void) {
    clearScreen(); // Clear the previous frame
    drawGround_LV2(); // Draw the ground
}

void reset_LV2(void) {
    renderBackGround_LV2(); // Render the background
    drawMario();
    defineObstacles_LV2(); // after reset, define obstacle again
    isFallingHole = 0;
}

void drawGround_LV2(void) {
    int ground_width = PHYSICAL_WINDOW_WIDTH - terrian22.groundPos.X;
    int ground_height = PHYSICAL_WINDOW_HEIGHT - terrian22.groundPos.Y;

    // set ground instance for terrian 22:
    setGroundObject(&terrian22, GND_X_POS, GND_Y_POS, ground_width, ground_height); // create base ground object instance of
    displayObject(terrian22.groundPos.X, terrian22.groundPos.Y, terrian22_terrian22, TERRIAN22_WIDTH, TERRIAN22_HEIGHT); // Draw ground

    // set ground instance for terrian 23:
    setGroundObject(&terrian23, terrian22.groundPos.X + ground2X[2], GND_Y_POS - 90, TERRIAN23_WIDTH, TERRIAN23_HEIGHT); // the terrian2 is 90 before the base ground
    displayObject(terrian23.groundPos.X, terrian23.groundPos.Y, terrian23_terrian23, TERRIAN23_WIDTH, TERRIAN23_HEIGHT);

    // set ground instance for terrian 25:
    setGroundObject(&terrian25, terrian22.groundPos.X + ground2X[3], GND_Y_POS - 90, TERRIAN25_WIDTH, TERRIAN25_HEIGHT); // the terrian2 is 90 before the base ground
    displayObject(terrian25.groundPos.X, terrian25.groundPos.Y, terrian25_terrian25, TERRIAN25_WIDTH, TERRIAN25_HEIGHT);
}

void defineObstacles_LV2(void) {
    // setObStacleObject(&terrian2_obstacle, terrian2.groundPos.X + TERRIAN2_OBSTACLE_X_OFFSET, terrian2.groundPos.Y + TERRIAN2_OBSTACLE_Y_OFFSET, OBJECT_WIDTH, OBJECT_HEIGHT);
    // setObStacleObject(&terrian2_stair, terrian2.groundPos.X + TERRIAN2_STAIR_X_OFFSET, terrian2.groundPos.Y + TERRIAN2_STAIR_Y_OFFSET, STAIR_WIDTH, STAIR_HEIGHT);
    // setObStacleObject(&terrian3_stair, terrian3.groundPos.X + TERRIAN3_STAIR_X_OFFSET, terrian2.groundPos.Y + TERRIAN3_STAIR_Y_OFFSET, STAIR_WIDTH, STAIR_HEIGHT);
}

void update_terrian_base_LV2(void) {
    terrian22.groundPos.X -= TRANSITION_OFF;
    setGroundObject(&terrian23, terrian23.groundPos.X + ground2X [1], GND_Y_POS - 90, TERRIAN23_WIDTH, TERRIAN23_HEIGHT); // update terrian 23 based on terrian 22
    setGroundObject(&terrian25, terrian25.groundPos.X + ground2X [2], GND_Y_POS - 90, TERRIAN25_WIDTH, TERRIAN25_HEIGHT); // update terrian 25 based on terrian 22
    setGroundObject(&terrian28, terrian28.groundPos.X + ground2X [3], GND_Y_POS - 90, TERRIAN28_WIDTH, TERRIAN28_HEIGHT); // update terrian 28 based on terrian 22
    setGroundObject(&terrian29, terrian29.groundPos.X + ground2X [4], GND_Y_POS - 90, TERRIAN29_WIDTH, TERRIAN29_HEIGHT); // update terrian 29 based on terrian 22
    setGroundObject(&ground_obj, ground_obj.groundPos.X + ground2X [5], GND_Y_POS - 90, TERRIAN1_WIDTH, TERRIAN1_HEIGHT); // update terrian 1 based on terrian 22 (terrian 30 is terrian 1)

    defineObstacles(); //redefine obstacle based on the terrian 2 
}

void ground_transition_handle_LV2(void) {
    displayObject(terrian22.groundPos.X + ground2X[0], terrian22.groundPos.Y + ground2Y[0], terrian22_terrian22, TERRIAN22_WIDTH, TERRIAN22_HEIGHT);
    displayObject(terrian22.groundPos.X + ground2X[1], terrian22.groundPos.Y + ground2Y[1], terrian23_terrian23,TERRIAN23_WIDTH, TERRIAN23_HEIGHT);
    displayObject(terrian22.groundPos.X + ground2X[2], terrian22.groundPos.Y + ground2Y[2], terrian25_terrian25,TERRIAN25_WIDTH, TERRIAN25_HEIGHT);
    displayObject(terrian22.groundPos.X + ground2X[3], terrian22.groundPos.Y + ground2Y[3], terrian28_terrian28, TERRIAN28_WIDTH, TERRIAN28_HEIGHT);
    displayObject(terrian22.groundPos.X + ground2X[4], terrian22.groundPos.Y + ground2Y[4], terrian29_terrian29,TERRIAN29_WIDTH, TERRIAN29_HEIGHT);
    displayObject(terrian22.groundPos.X + ground2X[5], terrian22.groundPos.Y + ground2Y[5], terrian1_terrian1,TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    
}

void moveObstacleToLeft_LV2(void) {
    deleteAnimationFrame(terrian22.groundPos.X + ground2X[0], terrian22.groundPos.Y + ground2Y[0], terrian22_terrian22, TERRIAN22_WIDTH, TERRIAN22_HEIGHT);
    deleteAnimationFrame(terrian22.groundPos.X + ground2X[1], terrian22.groundPos.Y + ground2Y[1], terrian23_terrian23,TERRIAN23_WIDTH, TERRIAN23_HEIGHT);
    deleteAnimationFrame(terrian22.groundPos.X + ground2X[2], terrian22.groundPos.Y + ground2Y[2], terrian25_terrian25,TERRIAN25_WIDTH, TERRIAN25_HEIGHT);
    deleteAnimationFrame(terrian22.groundPos.X + ground2X[3], terrian22.groundPos.Y + ground2Y[3], terrian28_terrian28, TERRIAN28_WIDTH, TERRIAN28_HEIGHT);
    deleteAnimationFrame(terrian22.groundPos.X + ground2X[4], terrian22.groundPos.Y + ground2Y[4], terrian29_terrian29,TERRIAN29_WIDTH, TERRIAN29_HEIGHT);
    deleteAnimationFrame(terrian22.groundPos.X + ground2X[5], terrian22.groundPos.Y + ground2Y[5], terrian1_terrian1,TERRIAN1_WIDTH, TERRIAN1_HEIGHT);


    // handle ground transition
    update_terrian_base_LV2();
    ground_transition_handle_LV2();   
}

void handleSceneTransition(void) {
    if (mario_char.currentPos.X > SCENE_TRANSITION_X) {
        isReachTransition = 1;
        mario_char.currentPos.X = SCENE_TRANSITION_X;
        setMarioHitBox();
        if(isHitObstacle == 0) {
            //moveObstacleToLeft(); // LEVEL 1
            lv2 ();
        }
    } else { // This 'else' handles BOTH scenarios
        if (mario_char.currentPos.X < INITIAL_POSITION_X) {
            mario_char.currentPos.X = INITIAL_POSITION_X;
        }
        isReachTransition = 0; // Reset only if Mario isn't transitioning
    }
}

int detect_black_pixel(mario_t tmp_char, unsigned long *terrian, ground_t tmp_ground) {
    int marioLeft = tmp_char.marioHitBox.top_left_corner.X - tmp_ground.groundPos.X;
    int marioRight = tmp_char.marioHitBox.top_left_corner.X + tmp_char.marioHitBox.width;
    int marioTop = tmp_char.marioHitBox.top_left_corner.Y;
    int marioBottom = tmp_char.marioHitBox.top_left_corner.Y + tmp_char.marioHitBox.height - tmp_ground.groundPos.Y;
    
    int indx = marioLeft * marioBottom;
    if(terrian[indx] == 0x00000000000) return 1;
}