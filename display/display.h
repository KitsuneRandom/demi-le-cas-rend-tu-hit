#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/utils.h"

// Fonction qui renvoie le nombre de cractères occupés par l'entier
int number_of_char(const uint32_t i);

// Affiche le séparateur entre 2 nombres
// Paramètres : new_line = true si retour à la ligne
void print_separator(bool new_line);

// Fonctions permettant d'afficher les lignes entre les cases
void print_first_line();
void print_middle_line();
void print_last_line();

// Affiche le nombre de la bonne couleur
void print_number(const uint32_t n);

// Affiche la grille de jeu à l'aide des fonctions précédentes
void print_grid(const uint32_t *grid);

/*
Dimensions :
------------

<-> : 37 cases (8x4 + 5 interlignes)
^
│   : 17 cases (3x4 + 5 interlignes)
v

intérieur des cases :
---------------------

2 <= x <= 8 : "    " + x + "   "
16 <= x <= 64 : "   " + x + "   "
128 <= x <= 512 : "   " + x + "  "
x >= 1024 : "  " + x + "  "

Maquette :
----------

┌────────┬────────┬────────┬────────┐
│        │        │        │        │
│    2   │        │        │        │
│        │        │        │        │
├────────┼────────┼────────┼────────┤
│        │        │        │        │
│        │        │        │        │
│        │        │        │        │
├────────┼────────┼────────┼────────┤
│        │        │        │        │
│   128  │        │        │        │
│        │        │        │        │
├────────┼────────┼────────┼────────┤
│        │        │        │        │
│  1024  │   64   │   32   │    8   │
│        │        │        │        │
└────────┴────────┴────────┴────────┘

Couleurs :
----------

Lignes : none
2 : none
4 : BOLDWHITE
8 : YELLOW
16 : GREEN
32 : CYAN
64 : BLUE
128 : MAGENTA
256 : RED
512 : BOLDRED
1024 : BOLDGREEN
2048 : BOLDWHITE
*/