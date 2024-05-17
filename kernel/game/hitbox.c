#include "hitbox.h"


void changeBoxSize(coordinate_t *dat, coordinate_t object_pos, int width_size, int height_size, int coord_pos) {
    if (coord_pos == TOP_LEFT_CORNER) {
        dat->X = object_pos.X + HITBOX_OFFSET;
        dat->Y = object_pos.Y + HITBOX_OFFSET;
    } else if (coord_pos == TOP_RIGHT_CORNER) {
        dat->X = (object_pos.X + width_size) - HITBOX_OFFSET;
        dat->Y = object_pos.Y + HITBOX_OFFSET;
    } else if (coord_pos == BOTTOM_LEFT_CORNER) {
        dat->X = object_pos.X + HITBOX_OFFSET;
        dat->Y = height_size - HITBOX_OFFSET;
    } else if (coord_pos == BOTTOM_RIGHT_CORNER) {
        dat->X = (object_pos.X + width_size) - HITBOX_OFFSET;
        dat->Y = height_size - HITBOX_OFFSET;
    }
}