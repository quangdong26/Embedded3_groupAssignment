#include "ground.h"

// Hitbox offset
#define HITBOX_OFFSET 10

#define DEFAULT 0

// Hitbox struct
typedef struct {
    coordinate_t top_left_corner;
    coordinate_t top_right_corner;
    coordinate_t bottom_left_corner;
    coordinate_t bottom_right_corner;
    volatile int height;
    volatile int width;
} hitbox_t;

void changeBoxSize(coordinate_t *dat, coordinate_t object_pos, int width_size, int height_size, int coord_pos, int hitbox_offs);