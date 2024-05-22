#include "obstacle.h"
obstacle_t mario_obstacle;

void setObStacleObject(void) {
    // randomly generate obstacle
    unsigned int range = MAX_OBSTACLE_X - MIN_OBSTACLE_X + 1;
    unsigned int randomX = MIN_OBSTACLE_X + (lcg_random() % range);
    // assigned the value for position
    mario_obstacle.obstaclePos.X = randomX;
    mario_obstacle.obstaclePos.Y = ground_obj.groundPos.Y - OBSTACLE_HEIGHT;
    mario_obstacle.height = OBSTACLE_HEIGHT;
    mario_obstacle.width = OBSTACLE_WIDTH;
    setObstacleHitBox();
}

void setObstacleHitBox(void) {
    mario_obstacle.obstacleHitBox.top_left_corner.X = mario_obstacle.obstaclePos.X;
    mario_obstacle.obstacleHitBox.top_left_corner.Y = mario_obstacle.obstaclePos.Y;
    mario_obstacle.obstacleHitBox.width = mario_obstacle.width;
    mario_obstacle.obstacleHitBox.height = mario_obstacle.height;
}