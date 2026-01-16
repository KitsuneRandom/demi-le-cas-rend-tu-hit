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
void init_grid(int* grid);

// desinit
void desinit_grid(int* grid);

int get_cell_value(int* grid, int cell);
void set_cell_value(int* grid, int cell, int value);

// recherche des cases vides
// retourne le nombre de cellules vides
int number_of_empty_cells(int* grid);

// ajout d'une nouvelle case
void add_random_cell(int* grid);

// décalage d'une case
void move_cell_up(int* grid, int cell);
void move_cell_right(int* grid, int cell);
void move_cell_down(int* grid, int cell);
void move_cell_left(int* grid, int cell);
void move_cell(int* grid, int cell, enum directions dir);

// fusion de 2 cases
void merge_cells(int* grid, int cella, int cellb);

// décaler toutes les cases
void move_all(int* grid, enum directions dir);
