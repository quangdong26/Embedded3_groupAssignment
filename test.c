#include <stdio.h>

#define A 1103515245
#define C 12345
#define M 2147483648

unsigned int seed = 0;  // Zero seed

unsigned int lcg_rand() {
    seed = (A * seed + C) % M;
    return seed;
}

void srand(unsigned int new_seed) {
    seed = new_seed;
}

int main() {
    int iSecret, iGuess;

    srand(123456789);  // Zero seed, starting sequence with C

    iSecret = lcg_rand() % 10 + 1;

    do {
        printf("Guess the number (1 to 10): ");
        scanf("%d", &iGuess);
        if (iSecret < iGuess) puts("The secret number is lower");
        else if (iSecret > iGuess) puts("The secret number is higher");
    } while (iSecret != iGuess);

    puts("Congratulations!");
    return 0;
}
