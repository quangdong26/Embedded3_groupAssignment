#include "../framebuffer/framebf.h"
#include "../background/background2.h"
#include "../image/mario.h"
#include "../uart/uart0.h"
#include "../uart/uart1.h"

#define BACKGROUND1_HEIGHT 224
#define BACKGROUND1_WIDTH 3268
#define ENTIRE_SCREEN 3000

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

//Define ground position
#define GND_X_POS 0
#define GND_Y_POS 600

// game state
extern volatile int gameState;
extern volatile int isGameInit;

// coordinate struct
typedef struct {
    volatile int X;
    volatile int Y;
} coordinate_t;

// mario struct
typedef struct {
    coordinate_t currentPos;
    coordinate_t pastPos;
    volatile int width_size;
    volatile int height_size;
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
    volatile int width;
    volatile int height;
} obstacle_t;

extern mario_t mario_char;
extern ground_t ground_obj;
extern obstacle_t mario_obstacle;

// function prototype
void gameOn(char c);
void drawGround(void);