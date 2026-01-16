#include "game_feature.h"

// init
void init_grid(int* grid) {
  grid = malloc(GRID_SIZE*sizeof(int));
  if (grid == NULL) {
    exit(EXIT_FAILURE);
  }
}

// desinit
void desinit_grid(int* grid) {
  free(grid);
  grid = NULL;
}

int get_cell_value(int* grid, int cell) {
  return grid[cell];
}

void set_cell_value(int* grid, int cell, int value) {
  grid[cell] = value;
}

// recherche des cases vides
// retourne le nombre de cellules vides
int number_of_empty_cells(int* grid) {
  int number_empty = 0;
  for (int i = 0; i < GRID_SIZE; i++) {
    if (get_cell_value(grid, i) == 0) {
      number_empty++;
    }
  }
  return number_empty;
}

// choisit une case vide au hasard
int choose_random_empty_cell(int* grid) {
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
void add_random_cell(int* grid) {
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
void move_cell_up(int* grid, int cell) {
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

void move_cell_right(int* grid, int cell) {
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

void move_cell_down(int* grid, int cell) {
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

void move_cell_left(int* grid, int cell) {
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

void move_cell(int* grid, int cell, directions dir) {
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
void merge_cells(int* grid, int cella, int cellb) {

}

// décaler toutes les cases
void move_all(int* grid, directions dir) {

}
