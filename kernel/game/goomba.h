#include "mario.h"

// Define character size
#define OBJECT_HEIGHT 32
#define OBJECT_WIDTH 32

// Define delay
#define FRAME_DELAY 50000  // Update position every 50000 frames

// Goomba struct
typedef struct {
    coordinate_t currentPos;
    coordinate_t pastPos;
    hitbox_t goombaHitBox;
    volatile int width_size;
    volatile int height_size;
} goomba_t;

extern goomba_t goomba_char;
extern goomba_t goomba2;
extern volatile int goomba_frame_counter;


// // frame counter flag
// extern volatile int frameCounter; // this can be changed in the game logic


// Function declaration
void setGoombaHitBox(void);
void renderGoombaInitPoint(void);
void handleLeftMovement(void);
void initStatGoomba(void);
void renderGoombaCurrentPoint(int x);