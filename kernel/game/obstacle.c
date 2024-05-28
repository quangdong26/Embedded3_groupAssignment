#include "obstacle.h"
obstacle_t terrian2_obstacle;
obstacle_t terrian2_stair;
obstacle_t terrian3_stair;
obstacle_t terrian11_square;

void setObStacleObject(obstacle_t *tmp, int x, int y, int width, int height) {
    // assigned the value for position
    tmp->obstaclePos.X = x;
    tmp->obstaclePos.Y = y;
    tmp->height = height;
    tmp->width = width;
    setObstacleHitBox(tmp);
}

void setObstacleHitBox(obstacle_t *tmp) {
    tmp->obstacleHitBox.top_left_corner.X = tmp->obstaclePos.X;
    tmp->obstacleHitBox.top_left_corner.Y = tmp->obstaclePos.Y;
    tmp->obstacleHitBox.width = tmp->height;
    tmp->obstacleHitBox.height = tmp->width;
}