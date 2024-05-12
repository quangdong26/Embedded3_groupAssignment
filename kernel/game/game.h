#include "../framebuffer/framebf.h"
#include "../background/background2.h"
#include "../image/mario.h"

#define BACKGROUND1_HEIGHT 224
#define BACKGROUND1_WIDTH 3268
#define BACKGROUND2_HEIGHT 456
#define BACKGROUND2_WIDTH 3390
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

extern mario_t mario_char;


void gameOn(char c);