#include "../framebuffer/framebf.h"
#include "../background/background2.h"
#include "../image/mario.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"

#define BACKGROUND1_HEIGHT 224
#define BACKGROUND1_WIDTH 3268
#define ENTIRE_SCREEN 3000

// define Left/right coordinate
#define TOP_LEFT_CORNER 0
#define TOP_RIGHT_CORNER 1

// define top/bot 
#define BOTTOM_LEFT_CORNER 0
#define BOTTOM_RIGHT_CORNER 1

#define JUMP_VELOCITY 25  // Higher initial jump velocity, change this if you want mario to jump higher or lower
#define GRAVITY 1         // Gravity applied each frame, change this if you want mario to fall faster or slower

#define FRAME_DELAY 50000  // Update position every 50000 frames

//State define
#define DEFAULT 0
#define GAME_ON 1
#define GAME_OFF 0

// check if the game is init
#define INIT 1

//Define character size
#define OBJECT_HEIGHT 203
#define OBJECT_WIDTH 206
#define OBSTACLE_WIDTH 150
#define OBSTACLE_HEIGHT 150

// hitbox offset
#define HITBOX_OFFSET 20

//Define ground position
#define GND_X_POS 0
#define GND_Y_POS 600

// plus or minus new value
#define INCREMENT_VAL 1
#define DECREMENT_VAL -1

// game state
extern volatile int gameState;
extern volatile int isGameInit;

// coordinate struct
typedef struct {
    volatile int X;
    volatile int Y;
} coordinate_t;

// hitbox struct
typedef struct {
    coordinate_t top_left_corner;
    coordinate_t top_right_corner;
    coordinate_t bottom_left_corner;
    coordinate_t bottom_right_corner;
} hitbox_t;

// mario struct
typedef struct {
    coordinate_t currentPos;
    coordinate_t pastPos;
    hitbox_t marioHitBox;
    volatile int width_size;
    volatile int height_size;
    int isJumping; // Flag to check if Mario is currently jumping
    int jumpVelocity; // Current jump velocity
} mario_t;

// mario action
typedef enum {
    MOVE_LEFT,
    MOVE_RIGHT,
    JUMP,
    CROUCH
} MarioAction;

//ground struct
typedef struct {
    coordinate_t groundPos;
    volatile int width;
    volatile int height;

} ground_t;

//obstacle
typedef struct {
    coordinate_t obstaclePos;
    hitbox_t obstacleHitBox;
    volatile int width;
    volatile int height;
} obstacle_t;

extern mario_t mario_char;
extern ground_t ground_obj;
extern obstacle_t mario_obstacle;

// Function prototype
void gameOn(void);
void setHitBox(int objLen);
void drawGround(void);
