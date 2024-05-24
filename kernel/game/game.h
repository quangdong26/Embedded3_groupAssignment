
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "obstacle.h"



//define transition offset
#define TRANSITION_OFF 5

#define INPUT_DELAY 1

// State define
#define DEFAULT 0
#define GAME_ON 1
#define GAME_OFF 0

// Check if the game is init
#define INIT 1


// Plus or minus new value
#define INCREMENT_VAL 1
#define DECREMENT_VAL -1

#define SCENE_TRANSITION_X 350  // Transition point


// game state
extern volatile int gameState;
extern volatile int isGameInit;
//extern int extra_shadow;

// Function prototypes
void gameOn(void);
void drawGround(void);