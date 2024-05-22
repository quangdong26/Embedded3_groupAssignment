#include "hitbox.h"


// void changeBoxSize(coordinate_t *dat, coordinate_t object_pos, int width_size, int height_size, int coord_pos, int hitbox_offs) {
//     switch(coord_pos) {
//         case TOP_LEFT_CORNER:
//             dat->X = object_pos.X + hitbox_offs;
//             dat->Y = object_pos.Y + hitbox_offs;
//             break;
//         case TOP_RIGHT_CORNER:
//             dat->X = (object_pos.X + width_size) - hitbox_offs;
//             dat->Y = object_pos.Y + hitbox_offs;
//             break;
//         case BOTTOM_LEFT_CORNER:
//             dat->X = object_pos.X + hitbox_offs;
//             dat->Y = (object_pos.Y + height_size) - hitbox_offs;
//             break;
//         case BOTTOM_RIGHT_CORNER:
//             dat->X = (object_pos.X + width_size) - hitbox_offs;
//             dat->Y = (object_pos.Y + height_size) - hitbox_offs;
//             break;
//     }
// }