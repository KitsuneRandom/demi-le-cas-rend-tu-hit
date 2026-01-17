#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// Grille : tableau de 16 entiers
//  - 0 si case vide
//  - 2, 4, 8, 16, ...
// Quand 2048 -> Gagné
// Cellule : représentée par son indice dans le tableau.
// Pour trouver la valeur : grid[x]

#define GRID_SIZE 16

// enum pour les différentes directions
typedef enum directions { 
    Up = 0, Right, Down, Left
} directions;

// init
void init_grid(uint16_t** grid);

// desinit
void desinit_grid(uint16_t** grid);

int get_cell_value(uint16_t* grid, int cell);
void set_cell_value(uint16_t* grid, int cell, int value);

// recherche des cases vides
// retourne le nombre de cellules vides
int number_of_empty_cells(uint16_t* grid);

// ajout d'une nouvelle case
bool add_random_cell(uint16_t* grid);

// Compacte une ligne ou une colonne en fonction de la direction du déplacement
void compact_line(uint16_t* line);

// décaler toutes les cases
void move_all(uint16_t* grid, enum directions dir);

// Vérifie si le joueur a gagné
bool check_win(uint16_t* grid);