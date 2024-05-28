#include "obstacle.h"

// Define character size
#define OBJECT_HEIGHT 32 //change as below for the small mario
#define OBJECT_WIDTH 32 //change as below for the small mario
// #define OBJECT_HEIGHT 32
// #define OBJECT_WIDTH 32

#define JUMP_VELOCITY 25  // Higher initial jump velocity, change this if you want Mario to jump higher or lower
#define GRAVITY 1         // Gravity applied each frame, change this if you want Mario to fall faster or slower

// Define delay
#define FRAME_DELAY 50000  // Update position every 50000 frames

#define INITIAL_POSITION_X 0

// define the valley in the terrian 3
#define TERRIAN3_VALLEY_X 0
#define TERRIAN3_VALLEY_Y 168



// Mario action
typedef enum {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_LEFT_JUMP,
    MOVE_RIGHT_JUMP,
    JUMP,
    CROUCH
} MarioAction;

// Mario struct
typedef struct {
    coordinate_t currentPos;
    coordinate_t pastPos;
    hitbox_t marioHitBox;
    volatile int width_size;
    volatile int height_size;
    int isJumping; // Flag to check if Mario is currently jumping
    int jumpVelocity; // Current jump velocity
    int jumpDirection; // Direction of the jump: 0 = straight up, -1 = left, 1 = right
    float horizontalSpeed; // Horizontal speed for smooth movement
    int canDoubleJump; // Flag for double jump
} mario_t;

extern mario_t mario_char;

// scene scroller flag
extern volatile int isReachTransition; // this can be changed in game logic

// key flag
extern int w_pressed;
extern int a_pressed;
extern int s_pressed;
extern int d_pressed;

// frame counter flag
extern volatile int frameCounter; // this can be changed in the game logic

// stick on obstacle flag
extern volatile int isOnObstacle;

//check if enter the valley
extern volatile int isEnterValley;

// check if fallin the hole
extern volatile int isFallingHole;

// Function declaration
void setMarioHitBox(void);
void renderPlayerInitPoint(void);
void handleHorizontalMovement(MarioAction action);
void applyGravity(void);
void marioMovement(MarioAction action);
void initStatMario(void);
void handleJumping(void);
void handle_stay_on_obstacle(obstacle_t tmp);