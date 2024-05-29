# Terrain Object Initialization and Management

This guide will help you understand the process of defining and managing terrain objects and obstacles in a Mario-style game using C. Follow these steps to properly set up and handle terrain objects.

## 1. Initial Setup of Terrain Object

First, initialize the terrain object using the following code snippet:

```c
setGroundObject(&terrian3, ground_obj.groundPos.X + 3 * GND_LENGTH, GND_Y_POS - 90, TERRIAN3_WIDTH, TERRIAN3_HEIGHT);
```

## 2. Display the Terrain Object
Make sure to display the terrain object by placing the initialization code in the init condition of your program.

## 3. Open the Array Bit and Search for Color Code
To define specific bits in an array representing the terrain, follow these steps:

- Open the array bit file in Visual Studio Code (VSCode).
- Search for the color code, for example, 0x009c4a00.
- In Mario, colors are usually fixed within a range of 10 hexadecimal color codes.
Identify the color code in the array bit to determine the column and rows of the first pixel of the object.

## 4. Calculate the Pixel X Coordinate
Once you have determined the column of the character in the array bit, use the following formula to calculate the Pixel X coordinate:
```c
Pixel_X_Coordinate = (Column_of_Char) / 12
```
## 5. Set Object Instances
```c
void defineObstacles(void) {
    displayObject(goomba_char.currentPos.X, goomba_char.currentPos.Y, defaultGoomba, OBJECT_WIDTH, OBJECT_HEIGHT);
    setObStacleObject(&terrian2_obstacle, terrian2.groundPos.X + TERRIAN2_OBSTACLE_X_OFFSET, terrian2.groundPos.Y + TERRIAN2_OBSTACLE_Y_OFFSET, OBJECT_WIDTH, OBJECT_HEIGHT);
    setObStacleObject(&terrian2_stair, terrian2.groundPos.X + TERRIAN2_STAIR_X_OFFSET, terrian2.groundPos.Y + TERRIAN2_STAIR_Y_OFFSET, STAIR_WIDTH, STAIR_HEIGHT);
    setObStacleObject(&terrian3_stair, terrian3.groundPos.X + TERRIAN3_STAIR_X_OFFSET, terrian3.groundPos.Y + TERRIAN3_STAIR_Y_OFFSET, STAIR_WIDTH, STAIR_HEIGHT);
    setObStacleObject(&terrian10_stair, terrian10.groundPos.X + TERRIAN10_STAIR_X_OFFSET, terrian10.groundPos.Y + TERRIAN10_STAIR_Y_OFFSET, STAIR_TERRIAN10_WIDTH, STAIR_TERRIAN10_HEIGHT);
}
```
## 6. Redefine Terrain and Objects in Movement
```c
void moveObstacleToLeft(void) {
    deleteAnimationFrame(ground_obj.groundPos.X + groundX[0], ground_obj.groundPos.Y + groundY[0], terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + groundX[1], ground_obj.groundPos.Y + groundY[1], terrian1_terrian1, TERRIAN1_WIDTH, TERRIAN1_HEIGHT);
    deleteAnimationFrame(terrian2.groundPos.X, terrian2.groundPos.Y + groundY[2], terrian2_terrian2, TERRIAN2_WIDTH, TERRIAN2_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + groundX[2], ground_obj.groundPos.Y + groundY[3], terrian3_terrian3, TERRIAN3_WIDTH, TERRIAN3_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + groundX[3], ground_obj.groundPos.Y - TERRIAN10_SCENE_Y, terrian10_terrian10, TERRIAN10_WIDTH, TERRIAN10_HEIGHT);
    deleteAnimationFrame(ground_obj.groundPos.X + groundX[4], ground_obj.groundPos.Y - TERRIAN11_SCENE_Y, terrian11_terrian11, TERRIAN11_WIDTH, TERRIAN11_HEIGHT);

    // Handle ground transition
    update_terrian_base();
    ground_transition_handle();
}
```