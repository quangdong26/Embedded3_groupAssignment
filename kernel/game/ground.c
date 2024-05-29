#include "ground.h"
ground_t ground_obj;
ground_t terrian2;
ground_t terrian3;
ground_t terrian10;
ground_t terrian11;

ground_t terrian22;
ground_t terrian23;
ground_t terrian25;
ground_t terrian28;
ground_t terrian29;

void setGroundObject(ground_t *tmp, int x, int y, int width, int height) {
    tmp->groundPos.X = x;
    tmp->groundPos.Y = y;
    tmp->width = width;
    tmp->height = height;
}