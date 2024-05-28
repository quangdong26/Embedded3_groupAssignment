#include "ground.h"
ground_t ground_obj;
ground_t terrian2;
ground_t terrian3;
ground_t terrian10;
ground_t terrian11;

void setGroundObject(ground_t *tmp, int x, int y, int width, int height) {
    tmp->groundPos.X = x;
    tmp->groundPos.Y = y;
    tmp->width = width;
    tmp->height = height;
}