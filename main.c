#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"

int main(void) {
    // Grille de test (4x4)
    uint32_t grid[16] = {
        2,    4,    8,    16,
        32,   64,   128,  256,
        512,  1024, 2048, 0,
        0,    2,    4,    8
    };

    // Affichage de la grille
    print_grid(grid);

    return 0;
}
