
#include "./hitbox.h"

#define OBSTACLE_WIDTH 60
#define OBSTACLE_HEIGHT 61

// Define obstacle position interval
#define MAX_OBSTACLE_X (PHYSICAL_WINDOW_WIDTH - 150)
#define MIN_OBSTACLE_X (PHYSICAL_WINDOW_WIDTH / 2)

// define the x,y coordinate of the obstacle in terrian 2
#define TERR2_COORDINATE_X 194
#define TERR2_COORDINATE_Y 37

//Define dimension for stair object
#define STAIR_WIDTH 99
#define STAIR_HEIGHT 98 // the difference between the top (0) and ground (98)

// Obstacle struct
typedef struct {
    coordinate_t obstaclePos;
    hitbox_t obstacleHitBox;
    volatile int width;
    volatile int height;
    volatile int isRightToMario;
} obstacle_t;

// function declaration
extern obstacle_t terrian2_obstacle;
extern obstacle_t terrian2_stair;
extern obstacle_t terrian3_stair;



void setObstacleHitBox(obstacle_t *tmp);

void setObStacleObject(obstacle_t *tmp, int x, int y, int width, int height);


