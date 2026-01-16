#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "game/game_feature.h"

int main(void) {
    srand(time(NULL));
    // Grille de test (4x4)
    /*uint32_t grid[16] = {
        2,    4,    8,    16,
        32,   64,   128,  256,
        512,  1024, 2048, 0,
        0,    2,    4,    8
    };*/
    uint16_t* grid = NULL;

    init_grid(&grid);
    print_grid(grid);
    desinit_grid(&grid);

    return 0;
}
