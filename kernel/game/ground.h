#include "../framebuffer/framebf.h"
#include "../utils/utils.h"

#include "../background/background2.h"
#include "../image/marioImg.h"


// Define ground position
#define GND_X_POS 0
#define GND_Y_POS 600


#define SCENE_TRANSITION_X 480  // Transition point

// define terrian 10 starting y
#define TERRIAN10_SCENE_Y 187

// define terrian 11 starting y
#define TERRIAN11_SCENE_Y 25

//define terrian 23 offset
#define TERRIAN23_OFFSET -50

#define TERRIAN25_Y_OFFSET 84

#define TERRIAN29_Y_OFFSET 165 

#define TERRIAN_23_H_OFFS (TERRIAN23_HEIGHT - (VIRTUAL_WINDOW_HEIGHT - (GND_Y_POS - TERRIAN23_OFFSET)))

#define LV2_MARIO_START_OFF -10
// define Left/right coordinate
enum corner_position {
    TOP_LEFT_CORNER,
    TOP_RIGHT_CORNER,
    BOTTOM_LEFT_CORNER,
    BOTTOM_RIGHT_CORNER
};

#define BACKGROUND1_HEIGHT 224
#define BACKGROUND1_WIDTH 3268
#define ENTIRE_SCREEN 3000

// define ground length
#define GND_LENGTH 480

// Coordinate struct
typedef struct {
    volatile int X;
    volatile int Y;
} coordinate_t;

// Ground struct
typedef struct {
    coordinate_t groundPos;
    volatile int width;
    volatile int height;
} ground_t;

extern ground_t ground_obj; //terrian 1 (= terrian 30)
extern ground_t terrian2;
extern ground_t terrian3;
extern ground_t terrian10;
extern ground_t terrian11;

extern ground_t terrian22;
extern ground_t terrian23;
extern ground_t terrian25;
extern ground_t terrian28;
extern ground_t terrian29;

void setGroundObject(ground_t *tmp, int x, int y, int width, int height);
void clearGroundObject(ground_t *tmp);