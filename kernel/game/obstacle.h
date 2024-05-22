
#include "./hitbox.h"

#define OBSTACLE_WIDTH 100
#define OBSTACLE_HEIGHT 100

// Define obstacle position interval
#define MAX_OBSTACLE_X (WINDOW_WIDTH - 150)
#define MIN_OBSTACLE_X (WINDOW_WIDTH / 2)

// Obstacle struct
typedef struct {
    coordinate_t obstaclePos;
    hitbox_t obstacleHitBox;
    volatile int width;
    volatile int height;
    volatile int isRightToMario;
} obstacle_t;

extern obstacle_t mario_obstacle;

void setObstacleHitBox(void);

void setObStacleObject(void);