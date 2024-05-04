// ----------------------------------- framebf.h -------------------------------------
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
//task 1
void displayImage(int x, int y, const unsigned long *image, int width, int height);
void deleteImage(int x, int y, int width, int height);
//task 3 
void drawChar(unsigned char ch, int x, int y, unsigned int attr, int zoom);
void drawString(int x, int y, char *str, unsigned int attr, int zoom);
void drawWhiteSquare(int x, int y, int size);