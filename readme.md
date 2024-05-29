# Building a Mario Game with Bare Metal Programming

Follow these steps to set up and build a Mario game using bare metal programming.

## Step 1: Create a `.env` File

Since QEMU (Quick Emulator) varies for different operating systems (Windows, Mac, etc.), we need to create a `.env` file to specify the environment settings.

## Step 2: Specify Arguments in the Code Snippet Below

Open your `.env` file and specify the following arguments:

```makefile
remove_command := rm
force := -f
rasp_window := raspi3
