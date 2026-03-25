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
void print_separator(bool new_line, int fd) {
    dprintf(fd, "│");
    if (new_line) {
        print_middle_line(fd);
    }
}

// Fonctions permettant d'afficher les lignes entre les cases
void print_first_line(int fd) {
    dprintf(fd, "┌────────┬────────┬────────┬────────┐\n│        │        │        │        │\n│");
}
void print_middle_line(int fd) {
    dprintf(fd, "\n│        │        │        │        │\n├────────┼────────┼────────┼────────┤\n│        │        │        │        │\n│");
}
void print_last_line(int fd) {
    dprintf(fd, "\n│        │        │        │        │\n└────────┴────────┴────────┴────────┘\n");
}

// Affiche le nombre de la bonne couleur
void print_number(const uint16_t n, int fd) {
    switch (n) {
        case 0:
            dprintf(fd, "        ");
            break;
        case 2:
            dprintf(fd, VIOLET_CLAIR "    2   " ANSI_RESET);
            break;
        case 4:
            dprintf(fd, VIOLET "    4   " ANSI_RESET);
            break;
        case 8:
            dprintf(fd, BLEU "    8   " ANSI_RESET);
            break;
        case 16:
            dprintf(fd, BLEU_CYAN "   16   " ANSI_RESET);
            break;
        case 32:
            dprintf(fd, CYAN "   32   " ANSI_RESET);
            break;
        case 64:
            dprintf(fd, VERT_CLAIR "   64   " ANSI_RESET);
            break;
        case 128:
            dprintf(fd, VERT "   128  " ANSI_RESET);
            break;
        case 256:
            dprintf(fd, JAUNE_CLAIR "   256  " ANSI_RESET);
            break;
        case 512:
            dprintf(fd, JAUNE "   512  " ANSI_RESET);
            break;
        case 1024:
            dprintf(fd, ORANGE "  1024  " ANSI_RESET);
            break;
        case 2048:
            dprintf(fd, ROUGE "  2048  " ANSI_RESET);
            break;
        default:
            dprintf(fd, " WHATTT ");
            break;
    }
}

// Affiche la grille de jeu à l'aide des fonctions précédentes
void print_grid(const uint16_t *grid, int fd) {
    print_first_line(fd);
    for (int i = 0; i < 16; i++) {
        print_number(grid[i], fd);
        print_separator(i % 4 == 3 && i != 15, fd);
    }
    print_last_line(fd);
}
