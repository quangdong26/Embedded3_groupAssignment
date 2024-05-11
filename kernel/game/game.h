#include "../framebuffer/framebf.h"
#include "../background/background2.h"
#include "../image/mario.h"

#define BACKGROUND1_HEIGHT 224
#define BACKGROUND1_WIDTH 3268
#define BACKGROUND2_HEIGHT 456
#define BACKGROUND2_WIDTH 3390

//State define
#define DEFAULT 0
#define GAME_ON 1
#define GAME_OFF 0
extern volatile int gameState;

void gameOn(char c);