#include "game_feature.h"


int get_cell_value(grid* g, int cell) {
  return g->cells[cell];
}

void set_cell_value(grid* g, int cell, int value) {
  g->cells[cell] = value;
}

// recherche des cases vides
// retourne le nombre de cellules vides
int number_of_empty_cells(grid* g) {
  int number_empty = 0;
  for (int i = 0; i < GRID_SIZE; i++) {
    if (get_cell_value(g, i) == 0) {
      number_empty++;
    }
  }
  return number_empty;
}

// choisit une case vide au hasard
int choose_random_empty_cell(grid* g) {
  int nb_empty = number_of_empty_cells(g);
  if (nb_empty == 0) {
    return -1;
  }
  int cell_chose = rand() % nb_empty + 1;
  for (int i = 0; i < GRID_SIZE; i++) {
    if (get_cell_value(g, i) == 0) {
      cell_chose--;
      if (cell_chose == 0) {
        return i;
      }
    }
  }
  return -1;
}

// ajout d'une nouvelle case
bool add_random_cell(grid* g) {
  int cell = choose_random_empty_cell(g);
  if (cell == -1) {
    return false;
  }
  int number = rand() % 2;
  if (number == 0) {
    set_cell_value(g, cell, 2);
  }
  else {
    set_cell_value(g, cell, 4);
  }
  return true;
}

// Compacte une ligne ou une colonne en fonction de la direction du déplacement
int compact_line(uint16_t* line) {
  int score = 0;
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
          score += new_line[i];
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
  return score;
}

// décaler toutes les cases
void move_all(grid* g, directions dir) {
  for (int i = 0; i < 4; i++) {
    uint16_t temp_line[4] = {0};
    // Extraire la ligne ou la colonne en fonction de la direction
    for (int j = 0; j < 4; j++) {
      switch (dir) {
        case Left:
          temp_line[j] = get_cell_value(g, i * 4 + j);
          break;
        case Right:
          temp_line[j] = get_cell_value(g, i * 4 + (3 - j));
          break;
        case Up:
          temp_line[j] = get_cell_value(g, j * 4 + i);
          break;
        case Down:
          temp_line[j] = get_cell_value(g, (3 - j) * 4 + i);
          break;
      }
    }
    g->score += compact_line(temp_line);
    // Réécrire la ligne ou la colonne dans la grille
    for (int j = 0; j < 4; j++) {
      switch (dir) {
        case Left:
          set_cell_value(g, i * 4 + j, temp_line[j]);
          break;
        case Right:
          set_cell_value(g, i * 4 + (3 - j), temp_line[j]);
          break;
        case Up:
          set_cell_value(g, j * 4 + i, temp_line[j]);
          break;
        case Down:
          set_cell_value(g, (3 - j) * 4 + i, temp_line[j]);
          break;
      }
    }
  }
}

// Vérifie si le joueur a gagné
bool check_win(grid* g) {
  for (int i = 0; i < GRID_SIZE; i++) {
    if (get_cell_value(g, i) == 2048) {
      return true;
    }
  }
  return false;
}

bool check_lose(grid* g) {
  if (number_of_empty_cells(g) > 0) {
    return false;
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      // Vérifier les lignes
      if (get_cell_value(g, i * 4 + j) == get_cell_value(g, i * 4 + (j + 1))) {
        return false;
      }
      // Vérifier les colonnes
      if (get_cell_value(g, j * 4 + i) == get_cell_value(g, (j + 1) * 4 + i)) {
        return false;
      }
    }
  }
  return true;
}

game_state check_game_state(grid* g) {
    if (check_win(g)) {
        return WIN;
    } else if (check_lose(g)) {
        return LOSE;
    } else {
        return ONGOING;
    }
}