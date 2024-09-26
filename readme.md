

# 🕹️ Mario Game with Bare Metal Programming

This project is an implementation of a **Mario Game** using **Bare Metal Programming** on a **Raspberry Pi**. The game runs directly on the hardware without an underlying operating system, leveraging the screen, keyboard input, and command-line interface for logging and debugging. 

<p align="center">
    <img src="[https://via.placeholder.com/800x400?text=Mario+Game+with+Bare+Metal+Programming](https://www.nintendo.com/eu/media/images/10_share_images/portals_3/2x1_SuperMarioHub_image1600w.jpg)" alt="Mario Game Banner"/>
</p>

---

## 🎮 Gameplay Overview

The player controls **Mario**, who must navigate through obstacles and enemies while attempting to reach the end of the level. Mario can move left, right, and jump to avoid obstacles and fall into valleys. The game environment scrolls to simulate Mario moving forward through the game world.

---

## 🚀 Getting Started

### Step 1: Create a `.env` File

Since **QEMU (Quick Emulator)** varies across operating systems (Windows, Mac, etc.), you need to create a `.env` file to specify your environment settings.

```bash
remove_command := rm
force := -f
rasp_window := raspi3
```

### Step 2: Build the Project

1. Set up the environment with the `.env` file.
2. Follow the instructions to build the game and load it on your Raspberry Pi.
3. Execute the game using QEMU or on actual Raspberry Pi hardware.

### Step 3: Playing the Game

- Press **'w'** to move Mario up, **'a'** to move left, **'d'** to move right, and **'s'** to scroll down.
- Mario can jump and avoid obstacles. The game ends when Mario either completes the level or falls into an obstacle.

---

## 🛠️ Features

### 1. **Background and Scrolling**

The game features a dynamically scrolling background, which creates the illusion that Mario is moving forward through the world while maintaining his central position on the screen.



### 2. **Obstacles and Enemy (Goombas)**

- **Obstacles**: Various objects are placed in Mario’s path, which he must avoid by jumping or moving.
- **Goombas**: Enemies Mario must evade. If Mario collides with them, the game ends.

### 3. **Character Control and Animation**

Mario’s movements, including jumping and running, are fully animated. The character responds to player inputs with smooth transitions between standing, running, and jumping animations.

---

## 🧑‍💻 Task Contribution Table

Each team member contributed equally to the development of the Mario game, focusing on different aspects of the project:

| Team Member | Contribution (%) | Task Description                                   |
|-------------|------------------|----------------------------------------------------|
| **Dinh Minh**   | 25%              | - Game logic for Mario (collision, scrolling, physics)  |
| **Luong**       | 25%              | - Goomba logic and Mario movement                 |
| **Dong**        | 25%              | - Rendering assets and bug fixing                 |
| **Shirin**      | 25%              | - Image rendering and Goomba collision            |

---

## 🎨 Graphics and Rendering

The project extends the screen drawing capabilities to display large images, videos, and fonts using **ARGB32 color encoding**. The **frame buffer** mechanism manages the pixel data directly, allowing for complex image rendering and scrolling.

### Image Display
- A large scrollable image can be navigated using the **w** (up) and **s** (down) keys.
  
### Video Playback
- A brief video can be played back on the screen, though it is limited to small, low-resolution clips.

### Font Display
- Fonts are displayed with **ARGB32 color values**, allowing each team member's name to be rendered in a different color on the game screen.

---

## 🎮 How the Game is Played

Mario’s movement is controlled by physics and key inputs:
- **Horizontal Movement**: Press 'a' to move left, 'd' to move right.
- **Jumping**: Press 'w' to jump. The jumping mechanism includes gravity, jump velocity, and the possibility of double jumping.
  


---

## 📊 Results

- **Background scrolling**: Seamlessly implemented to simulate movement.
- **Obstacles**: Displayed correctly with collision detection.
- **Mario Animation**: Mario moves and jumps as expected, with smooth sprite transitions.

---

## 🖥️ Technical Implementation

- **Language**: C
- **Platform**: Raspberry Pi (bare metal, no OS)
- **Graphics**: Frame buffer rendering with ARGB32 pixels
- **Input Handling**: Keyboard input processed in real-time for character control.

Key functions include:
- `renderBackGround()`: Renders the game background.
- `drawMario()`: Draws the Mario sprite at his current position.
- `applyGravity()`: Simulates gravity to update Mario's jump.
- `moveObstacleToLeft()`: Scrolls the background to simulate Mario moving forward.

