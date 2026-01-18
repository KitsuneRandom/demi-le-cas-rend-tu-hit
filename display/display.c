#include "display.h"

// Fonction qui renvoie le nombre de cractères occupés par l'entier
int number_of_char(const uint16_t i) {
    if (i == 0) {
        return 0;
    }
    int count = 0;
    uint16_t num = i;
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
void print_number(const uint16_t n) {
    switch (n) {
        case 0:
            printf("        ");
            break;
        case 2:
            printf(VIOLET_CLAIR "    2   " ANSI_RESET);
            break;
        case 4:
            printf(VIOLET "    4   " ANSI_RESET);
            break;
        case 8:
            printf(BLEU "    8   " ANSI_RESET);
            break;
        case 16:
            printf(BLEU_CYAN "   16   " ANSI_RESET);
            break;
        case 32:
            printf(CYAN "   32   " ANSI_RESET);
            break;
        case 64:
            printf(VERT_CLAIR "   64   " ANSI_RESET);
            break;
        case 128:
            printf(VERT "   128  " ANSI_RESET);
            break;
        case 256:
            printf(JAUNE_CLAIR "   256  " ANSI_RESET);
            break;
        case 512:
            printf(JAUNE "   512  " ANSI_RESET);
            break;
        case 1024:
            printf(ORANGE "  1024  " ANSI_RESET);
            break;
        case 2048:
            printf(ROUGE "  2048  " ANSI_RESET);
            break;
        default:
            printf(" WHATTT ");
            break;
    }
}

// Affiche la grille de jeu à l'aide des fonctions précédentes
void print_grid(const uint16_t *grid) {
    print_first_line();
    for (int i = 0; i < 16; i++) {
        print_number(grid[i]);
        print_separator(i % 4 == 3 && i != 15);
    }
    print_last_line();
}
