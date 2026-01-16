#include "display.h"

// Fonction qui renvoie le nombre de cractères occupés par l'entier
int number_of_char(const uint32_t i) {
    if (i == 0) {
        return 0;
    }
    int count = 0;
    uint32_t num = i;
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

// Affiche le séparateur entre 2 nombres
// Paramètres : new_line = true si retour à la ligne
void print_separator(bool new_line) {
    printf("│");
    if (new_line) {
        print_middle_line();
    }
}

// Fonctions permettant d'afficher les lignes entre les cases
void print_first_line() {
    printf("┌────────┬────────┬────────┬────────┐\n│        │        │        │        │\n│");
}
void print_middle_line() {
    printf("\n│        │        │        │        │\n├────────┼────────┼────────┼────────┤\n│        │        │        │        │\n│");
}
void print_last_line() {
    printf("\n│        │        │        │        │\n└────────┴────────┴────────┴────────┘\n");
}

// Affiche le nombre de la bonne couleur
void print_number(const uint32_t n) {
    switch (n) {
        case 0:
            printf("        ");
            break;
        case 2:
            printf("    %d   ", n);
            break;
        case 4:
            printf(BOLDWHITE("    %d   "), n);
            break;
        case 8:
            printf(YELLOW("    %d   "), n);
            break;
        case 16:
            printf(GREEN("   %d   "), n);
            break;
        case 32:
            printf(CYAN("   %d   "), n);
            break;
        case 64:
            printf(BLUE("   %d   "), n);
            break;
        case 128:
            printf(MAGENTA("   %d  "), n);
            break;
        case 256:
            printf(RED("   %d  "), n);
            break;
        case 512:
            printf(BOLDRED("   %d  "), n);
            break;
        case 1024:
            printf(BOLDGREEN("  %d  "), n);
            break;
        case 2048:
            printf(BOLDWHITE("  %d  "), n);
            break;
    }
}

// Affiche la grille de jeu à l'aide des fonctions précédentes
void print_grid(const uint32_t *grid) {
    print_first_line();
    for (int i = 0; i < 16; i++) {
        print_number(grid[i]);
        print_separator(i % 4 == 3 && i != 15);
    }
    print_last_line();
}
