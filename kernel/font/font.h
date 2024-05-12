#include "../gpio/gpio.h"
// Declare them as extern
extern unsigned char font[224][8];
extern unsigned int vgapal[];
enum {
    FONT_WIDTH     = 8,  // Width in pixels
    FONT_HEIGHT    = 8,  // Height in pixel
    FONT_BPG       = 8,  // Bytes per character glyph
    FONT_BPL       = 1,  // Bytes per line
    FONT_NUMGLYPHS = 224 // Total number of characters in the font
};
