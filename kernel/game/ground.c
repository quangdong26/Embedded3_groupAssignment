#include "ground.h"
ground_t ground_obj;

void setGroundObject(void) {
    ground_obj.groundPos.X = GND_X_POS;
    ground_obj.groundPos.Y = GND_Y_POS;
    ground_obj.width = PHYSICAL_WINDOW_WIDTH - ground_obj.groundPos.X;
    ground_obj.height = PHYSICAL_WINDOW_HEIGHT - ground_obj.groundPos.Y;
}