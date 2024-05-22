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
    changeBoxSize(&mario_obstacle.obstacleHitBox.bottom_left_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBSTACLE_HEIGHT, TOP_LEFT_CORNER, DEFAULT);
    changeBoxSize(&mario_obstacle.obstacleHitBox.bottom_right_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBSTACLE_HEIGHT, TOP_RIGHT_CORNER, DEFAULT);
    changeBoxSize(&mario_obstacle.obstacleHitBox.top_left_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBSTACLE_HEIGHT, BOTTOM_LEFT_CORNER, DEFAULT);
    changeBoxSize(&mario_obstacle.obstacleHitBox.top_right_corner, mario_obstacle.obstaclePos, OBSTACLE_WIDTH, OBSTACLE_HEIGHT, BOTTOM_RIGHT_CORNER, DEFAULT);
}