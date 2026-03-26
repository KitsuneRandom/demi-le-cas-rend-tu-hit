#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <semaphore.h>

// Grille : tableau de 16 entiers
//  - 0 si case vide
//  - 2, 4, 8, 16, ...
// Quand 2048 -> Gagné
// Cellule : représentée par son indice dans le tableau.
// Pour trouver la valeur : grid[x]

#define GRID_SIZE 16

typedef struct grid {
    uint16_t cells[GRID_SIZE];
    int score;
} grid;

// enum pour les différentes directions
typedef enum directions { 
    Up = 0, Right, Down, Left, WRONG
} directions;

typedef struct msg {
    pid_t pid;
    char tty[128];
    bool new_game;
    bool run;
    directions dir;
} msg;

typedef enum game_state {
    WIN,
    LOSE,
    ONGOING,
    DEAD
} game_state;

typedef struct display_msg {
    grid grid;
    game_state state;
    char tty[128];
} display_msg;


int get_cell_value(grid* g, int cell);
void set_cell_value(grid* g, int cell, int value);

// recherche des cases vides
// retourne le nombre de cellules vides
int number_of_empty_cells(grid* g);
// ajout d'une nouvelle case
bool add_random_cell(grid* g);

// Compacte une ligne ou une colonne en fonction de la direction du déplacement
int compact_line(uint16_t* line);

// décaler toutes les cases
void move_all(grid* g, directions dir);
// Vérifie si le joueur a gagné
bool check_win(grid* g);

bool check_lose(grid* g);

game_state check_game_state(grid* g);