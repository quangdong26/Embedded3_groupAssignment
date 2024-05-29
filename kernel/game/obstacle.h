
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
#define STAIR_TERRIAN10_HEIGHT (STAIR_HEIGHT *2)
#define STAIR_TERRIAN10_WIDTH (STAIR_WIDTH *2)

// Define he offset of obstacle position in the array bit
#define TERRIAN2_OBSTACLE_X_OFFSET 194
#define TERRIAN2_OBSTACLE_Y_OFFSET 37

#define TERRIAN2_STAIR_X_OFFSET 385
#define TERRIAN2_STAIR_Y_OFFSET 0

#define TERRIAN3_STAIR_X_OFFSET 170 // position (1924, 0) of the 
#define TERRIAN3_STAIR_Y_OFFSET 0


#define TERRIAN10_STAIR_X_OFFSET 288
#define TERRIAN10_STAIR_Y_OFFSET 0
#define TERRIAN10_VALLEY_X 10
#define TERRIAN10_VALLEY_Y 259
#define TERRIAN10_VALLEY_OFF 128

#define TERRIAN11_SQUARE_X_START_OFFSET 2038
#define TERRIAN11_SQUARE_Y_START_OFFSET 3

// define the valley in the terrian 3
#define TERRIAN3_VALLEY_X 0
#define TERRIAN3_VALLEY_Y 168

#define TERRIAN22_TREE_X 256
#define TERRIAN22_TREE_Y 80
#define TREE_WIDTH 122
#define TREE_HEIGHT 153

#define TERRIAN25_SQUARE_X 72
#define TERRIAN25_VALLEY_HEIGHT 207

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
extern obstacle_t terrian10_stair;
extern obstacle_t terrian22_tree;



void setObstacleHitBox(obstacle_t *tmp);

void setObStacleObject(obstacle_t *tmp, int x, int y, int width, int height);


