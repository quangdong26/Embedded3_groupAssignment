#include "goomba.h"
#include "../image/defaultGoomba.h"

goomba_t goomba_char;
goomba_t goomba_2;
volatile int goomba_frame_counter = 0;

int startX = 0;
// volatile int frameCounter = 0;

void handleLeftMovement(void) {
    float speed = -2.0; // Speed at which the Goomba moves left

    if (frameCounter % FRAME_DELAY == 0) {
        goomba_char.pastPos.X = goomba_char.currentPos.X;
        goomba_char.pastPos.Y = goomba_char.currentPos.Y;
        goomba_char.currentPos.X += speed; // Move Goomba left

        if (goomba_char.currentPos.X < 0) { // Prevent Goomba from moving out of bounds
            goomba_char.currentPos.X = 0;
            goomba_char.currentPos.Y = -300; //delete this mf by hiding him under the ground (he still there, but burried, no one remember)
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
    goomba_char.currentPos.X = terrian2_obstacle.obstaclePos.X - 80;
    goomba_char.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Goomba starts on the ground
    
}

void renderGoombaCurrentPoint(int x) {
    goomba_char.pastPos.X = goomba_char.currentPos.X;
    goomba_char.currentPos.X -= x;
    setGoombaHitBox();
}

setGoomba2HitBox(void) {
    goomba_2.goombaHitBox.top_left_corner.X = goomba_2.currentPos.X;
    goomba_2.goombaHitBox.top_left_corner.Y = goomba_2.currentPos.Y;
    goomba_2.goombaHitBox.width = goomba_2.width_size;
    goomba_2.goombaHitBox.height = goomba_2.height_size;
}

void handleLeftRightMovement(int ending_offset) {
    static float speed = -2.0; // Initial speed at which the Goomba moves left
    startX = goomba_2.currentPos.X; // Starting X position of the Goomba
    int endX = startX + ending_offset; // Ending X position of the interval (e.g., 100 units from the start point)

    if (frameCounter % FRAME_DELAY == 0) {
        goomba_2.pastPos.X = goomba_2.currentPos.X;
        goomba_2.pastPos.Y = goomba_2.currentPos.Y;
        goomba_2.currentPos.X += speed; // Move Goomba left or right based on speed

        // Check if Goomba reaches the start or end points and reverse direction
        if (goomba_2.currentPos.X <= startX || goomba_2.currentPos.X >= endX) {
            speed = -speed; // Reverse direction
        }

        deleteAnimationFrame(goomba_2.pastPos.X, goomba_2.pastPos.Y, defaultGoomba, OBJECT_WIDTH, OBJECT_HEIGHT);
        displayObject(goomba_2.currentPos.X, goomba_2.currentPos.Y, defaultGoomba, OBJECT_WIDTH, OBJECT_HEIGHT);
        setGoombaHitBox();
    }
}

void renderGoomba2InitPoint(void) {
    goomba_2.height_size = OBJECT_HEIGHT;
    goomba_2.width_size = OBJECT_WIDTH;
    goomba_2.currentPos.X = terrian10_stair.obstaclePos.X - 80;
    goomba_2.currentPos.Y = ground_obj.groundPos.Y - OBJECT_HEIGHT; // Goomba starts on the ground
}
