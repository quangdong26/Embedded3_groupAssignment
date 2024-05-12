#include "../framebuffer/framebf.h"
#include "../background/background2.h"
#include "../image/mario.h"

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

//Define ground position
#define GND_X_POS 0
#define GND_Y_POS 400

// game state
extern volatile int gameState;
extern volatile int isGameInit;

// mario struct

typedef struct {
    volatile int currentX;
    volatile int currentY;
    volatile int pastX;
    volatile int pastY;
} mario_t;

//ground struct
typedef struct {
    volatile int xPos;
    volatile int yPos;
    volatile int width;
    volatile int height;

} ground_t;

extern mario_t mario_char;
extern ground_t ground_obj;


void gameOn(char c);