
#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "goomba.h"


#include "../image/defaultMario.h"
#include "../image/mariofw.h"
#include "../image/terrian1.h"
#include "../image/terrian2.h"
#include "../image/terrian3.h"
#include "../image/terrian10.h"
#include "../image/terrian11.h"
#include "../image/defaultGoomba.h"
#include "../image/pole.h"
#include "../image/end.h"

#include "../image/terrian22.h"
#include "../image/terrian23.h"
#include "../image/terrian25.h"
#include "../image/terrian28.h"
#include "../image/terrian29.h"


//define transition offset
#define TRANSITION_OFF 5

#define INPUT_DELAY 1

// State define

#define GAME_ON 1
#define GAME_OFF 0

// Check if the game is init
#define INIT 1


// Plus or minus new value
#define INCREMENT_VAL 1
#define DECREMENT_VAL -1



// game state
extern volatile int gameState;
extern volatile int isGameInit;
//extern int extra_shadow;

// Function prototypes
void gameOn(void);
void drawGround(void);