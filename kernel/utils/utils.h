// Constants for a Linear Congruential Generator
// Constants for a simple linear congruential generator (LCG)
#define LCG_A 1664525  // Multiplier
#define LCG_C 1013904223 // Increment
#define LCG_M 4294967296 // Modulus (2^32)

unsigned int lcg_random();