#include "game_features.h"

// init
void init_grid(int* grid) {
  grid = malloc(GRID_SIZE*sizeof(int));
}

// desinit
void desinit_grid(int* grid) {
  free(grid);
  grid = NULL;
}

// recherche des cases vides
// retourne le nombre de cellules vides
int number_of_empty_cells(int* grid) {
  int number_empty = 0;
  for (int i = 0; i < GRID_SIZE; i++) {
    if (grid[i] == 0) {
      number_empty++;
    }
  }
  return number_empty;
}

// choisit une case vide au hasard
int choose_random_empty_cell(int* grid) {
  int nb_empty = number_of_empty_cells(grid);
  if (nb_empty == 0) {
    return 0;
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
}

// ajout d'une nouvelle case
void add_random_cell(int* grid) {
  int cell = choose_random_empty_cell(grid);
  int number = rand() % 2;
  if (number == 0) {
    grid[cell] = 2;
  }
  else {
    grid[cell] = 4;
  }
}

// décalage d'une case
void move_cell(int* grid, int cell, enum directions dir) {

}

// fusion de 2 cases
void merge_cells(int* grid, int cella, int cellb) {

}

// décaler toutes les cases
void move_all(int* grid, enum directions dir) {

}
