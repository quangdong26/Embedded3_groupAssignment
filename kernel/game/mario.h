#include "./hitbox.h"

// Define character size
#define OBJECT_HEIGHT 32 //change as below for the small mario
#define OBJECT_WIDTH 32 //change as below for the small mario
// #define OBJECT_HEIGHT 32
// #define OBJECT_WIDTH 32

// Define constants for parabolic trajectory
#define MAX_HORIZONTAL_DISTANCE 200 // Maximum horizontal distance Mario can travel in a jump
#define JUMP_INITIAL_VELOCITY 17 // Initial velocity for the jump
#define GRAVITY 1 // Gravity affecting the jump

// Define delay
#define FRAME_DELAY 50000  // Update position every 50000 frames

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

// Function declaration
void setMarioHitBox(void);
void renderPlayerInitPoint(void);
void handleHorizontalMovement(MarioAction action);
void applyGravity(void);
void marioMovement(MarioAction action);
void initStatMario(void);
void handleJumping(void);