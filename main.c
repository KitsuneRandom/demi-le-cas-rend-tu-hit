#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "game/game_feature.h"

int main(void) {
    srand(time(NULL));
    uint16_t* grid = NULL;

    init_grid(&grid);
    set_cell_value(grid, 0, 2); 
    set_cell_value(grid, 1, 4); 
    set_cell_value(grid, 2, 8); 
    set_cell_value(grid, 3, 16);
    set_cell_value(grid, 4, 32);
    set_cell_value(grid, 5, 64); 
    set_cell_value(grid, 6, 128);
    set_cell_value(grid, 7, 256);
    set_cell_value(grid, 8, 512);
    set_cell_value(grid, 9, 1024);
    set_cell_value(grid, 10, 2048); 

    print_grid(grid);
    desinit_grid(&grid);

    return 0;
}
