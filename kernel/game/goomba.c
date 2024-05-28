#include "goomba.h"
#include "../image/defaultGoomba.h"

goomba_t goomba_char;
// volatile int frameCounter = 0;

void handleLeftMovement(void) {
    float speed = -1.0; // Speed at which the Goomba moves left

    if (frameCounter % FRAME_DELAY == 0) {
        goomba_char.pastPos.X = goomba_char.currentPos.X;
        goomba_char.pastPos.Y = goomba_char.currentPos.Y;
        goomba_char.currentPos.X += speed; // Move Goomba left

        if (goomba_char.currentPos.X < 0) { // Prevent Goomba from moving out of bounds
            goomba_char.currentPos.X = 0;
        }

        deleteAnimationFrame(goomba_char.pastPos.X, goomba_char.pastPos.Y, defaultGoomba, OBJECT_WIDTH, OBJECT_HEIGHT);
        displayObject(goomba_char.currentPos.X, goomba_char.currentPos.Y, defaultGoomba, OBJECT_WIDTH, OBJECT_HEIGHT);
        setGoombaHitBox();
    }
}


void setGoombaHitBox(void) {
    // Set the hitbox to fit Goomba's current size
    goomba_char.goombaHitBox.top_left_corner.X = goomba_char.currentPos.X;
    goomba_char.goombaHitBox.top_left_corner.Y = goomba_char.currentPos.Y;
    goomba_char.goombaHitBox.width = goomba_char.width_size;
    goomba_char.goombaHitBox.height = goomba_char.height_size;
}

void renderGoombaInitPoint(void) {
    goomba_char.height_size = OBJECT_HEIGHT;
    goomba_char.width_size = OBJECT_WIDTH;
    goomba_char.currentPos.X = mario_char.currentPos.X + 100;
    goomba_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Goomba starts on the ground
    setGoombaHitBox();
}