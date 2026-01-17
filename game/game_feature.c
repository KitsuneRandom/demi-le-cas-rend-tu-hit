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
  int cell_chose = rand() % nb_empty + 1;
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
bool add_random_cell(uint16_t* grid) {
  int cell = choose_random_empty_cell(grid);
  if (cell == -1) {
    return false;
  }
  int number = rand() % 2;
  if (number == 0) {
    set_cell_value(grid, cell, 2);
  }
  else {
    set_cell_value(grid, cell, 4);
  }
  return true;
}

// Compacte une ligne ou une colonne en fonction de la direction du déplacement
void compact_line(uint16_t* line) {
  // Enlever les 0
  uint16_t new_line[4] = {0};
  int index = 0;
  for (int i = 0; i < 4; i++) {
      if (line[i] != 0) {
        new_line[index] = line[i];
        index++;
      }
  }
  // Fusionner les cases
  for (int i = 0; i < 3; i++) {
      if (new_line[i] != 0 && new_line[i] == new_line[i+1]) {
          new_line[i] *= 2;
          new_line[i+1] = 0;
      }
  }
  // Enlever les nouveaux 0 + remplir le reste avec des 0
  index = 0;
  for (int i = 0; i < 4; i++) {
      if (new_line[i] != 0) {
        line[index] = new_line[i];
        index++;
      }
  }
  for (int i = index; i < 4; i++) {
      line[i] = 0;
  }
}

// décaler toutes les cases
void move_all(uint16_t* grid, directions dir) {
  for (int i = 0; i < 4; i++) {
    uint16_t temp_line[4] = {0};
    // Extraire la ligne ou la colonne en fonction de la direction
    for (int j = 0; j < 4; j++) {
      switch (dir) {
        case Left:
          temp_line[j] = grid[i * 4 + j];
          break;
        case Right:
          temp_line[j] = grid[i * 4 + (3 - j)];
          break;
        case Up:
          temp_line[j] = grid[j * 4 + i];
          break;
        case Down:
          temp_line[j] = grid[(3 - j) * 4 + i];
          break;
      }
    }
    compact_line(temp_line);
    // Réécrire la ligne ou la colonne dans la grille
    for (int j = 0; j < 4; j++) {
      switch (dir) {
        case Left:
          grid[i * 4 + j] = temp_line[j];
          break;
        case Right:
          grid[i * 4 + (3 - j)] = temp_line[j];
          break;
        case Up:
          grid[j * 4 + i] = temp_line[j];
          break;
        case Down:
          grid[(3 - j) * 4 + i] = temp_line[j];
          break;
      }
    }
  }
}

// Vérifie si le joueur a gagné
bool check_win(uint16_t* grid) {
  for (int i = 0; i < GRID_SIZE; i++) {
    if (get_cell_value(grid, i) == 2048) {
      return true;
    }
  }
  return false;
}