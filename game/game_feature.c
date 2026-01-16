#include "game_feature.h"

// init
void init_grid(uint16_t** grid) {
    *grid = calloc(GRID_SIZE, sizeof(uint16_t)); // calloc met bien a 0 au lieu de tout et n'importe quoi comme malloc
    if (*grid == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
}


// desinit
void desinit_grid(uint16_t** grid) {
    free(*grid);
    *grid = NULL;
}


int get_cell_value(uint16_t* grid, int cell) {
  return grid[cell];
}

void set_cell_value(uint16_t* grid, int cell, int value) {
  grid[cell] = value;
}

// recherche des cases vides
// retourne le nombre de cellules vides
int number_of_empty_cells(uint16_t* grid) {
  int number_empty = 0;
  for (int i = 0; i < GRID_SIZE; i++) {
    if (get_cell_value(grid, i) == 0) {
      number_empty++;
    }
  }
  return number_empty;
}

// choisit une case vide au hasard
int choose_random_empty_cell(uint16_t* grid) {
  int nb_empty = number_of_empty_cells(grid);
  if (nb_empty == 0) {
    return -1;
  }
  int cell_chose = rand() % (nb_empty + 1);
  for (int i = 0; i < GRID_SIZE; i++) {
    if (grid[i] == 0) {
      cell_chose--;
      if (cell_chose == 0) {
        return i;
      }
    }
  }
  return -1;
}

// ajout d'une nouvelle case
void add_random_cell(uint16_t* grid) {
  int cell = choose_random_empty_cell(grid);
  int number = rand() % 2;
  if (number == 0) {
    set_cell_value(grid, cell, 2);
  }
  else {
    set_cell_value(grid, cell, 4);
  }
}

// décalage d'une case
void move_cell_up(uint16_t* grid, int cell) {
  int value = get_cell_value(grid, cell);
  if (value == 0) {
    return;
  }

  int row = cell / 4;
  int col = cell % 4;

  while (row > 0 && get_cell_value(grid, (row - 1) * 4 + col) == 0) {
    set_cell_value(grid, row * 4 + col, 0);
    row--;
    set_cell_value(grid, row * 4 + col, value);
  }
}

void move_cell_right(uint16_t* grid, int cell) {
  int value = get_cell_value(grid, cell);
  if (value == 0) {
    return;
  }

  int row = cell / 4;
  int col = cell % 4;

  while (col < 3 && get_cell_value(grid, (col + 1)) == 0) {
    set_cell_value(grid, row * 4 + col, 0);
    col++;
    set_cell_value(grid, row * 4 + col, value);
  }
}

void move_cell_down(uint16_t* grid, int cell) {
  int value = get_cell_value(grid, cell);
  if (value == 0) {
    return;
  }

  int row = cell / 4;
  int col = cell % 4;

  while (row < 3 && grid[(row + 1) * 4 + col] == 0) {
    set_cell_value(grid, row * 4 + col, 0);
    row++;
    set_cell_value(grid, row * 4 + col, value);
  }
}

void move_cell_left(uint16_t* grid, int cell) {
  int value = get_cell_value(grid, cell);
  if (value == 0) {
    return;
  }

  int row = cell / 4;
  int col = cell % 4;

  while (col > 0 && grid[row * 4 + (col - 1)] == 0) {
    set_cell_value(grid, row * 4 + col, 0);
    col--;
    set_cell_value(grid, row * 4 + col, value);
  }
}

void move_cell(uint16_t* grid, int cell, directions dir) {
  switch (dir) {
    case Up :
      move_cell_up(grid, cell);
      break;
    case Right :
      move_cell_right(grid, cell);
      break;
    case Down :
      move_cell_down(grid, cell);
      break;
    case Left :
      move_cell_left(grid, cell);
      break;
  }
}

// fusion de 2 cases
void merge_cells(uint16_t* grid, int cella, int cellb) {

}

// décaler toutes les cases
void move_all(uint16_t* grid, directions dir) {

}
