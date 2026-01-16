#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "game/game_feature.h"

char ask_user_input(void) {
    char input;
    char is_ok = 1;
    char c;
    while (is_ok != 0) {
        printf("Entrez Z (haut), Q (gauche), S (bas) ou D (droite) : ");
        scanf(" %c", &input);
        
        while ((c = getchar()) != '\n' && c != EOF); // Vider le reste de la saisie de l'utilisateur après le premier caractère lu
        
        if (input != 'Z' && input != 'Q' && input != 'S' && input != 'D') {
            printf("valeur saisie invalide.\n");
        } else {
            is_ok = 0;
        }
    }
    return input;
}

directions ask_user_dir(void) {
    char input = ask_user_input();
    switch (input) {
        case 'Z' :
            return Up;
        case 'Q' :
            return Left;
        case 'S' :
            return Down;
        case 'D' :
            return Right;
        default :
            return Up;
    }
}

int main(void) {
    srand(time(NULL));
    
    printf("Bienvenue dans ce super 2048 (super jsp mais en tt cas 2048)\nLe but du jeu c'est de pas perdre le jeu, et pour ça il faut éviter de remplir la grille\nVous allez donc devoir fusionner les cases de même valeur jusqu'à obtenir une case 2048\nVous utiliserez Z, Q, S, et D pour vous déplpacer en haut, à gauche, en bas et à droite\n");
    printf("Voici un exemple de grille : \n");
    uint16_t exemple_grid[16] = {
        0, 0, 2, 8,
        0, 0, 4, 32,
        0, 4, 16, 64,
        2, 8, 32, 128
    };

    // Affichage de la grille
    print_grid(exemple_grid);
    
    
    uint16_t* grid = NULL;

    init_grid(&grid);
    //print_grid(grid);
    char end_game = 0; // 0 : pas fini, 1 : fini gagné, 2 : fini perdu, autre : wtf
    while (end_game == 0) {
        add_random_cell(grid);
        print_grid(grid);    // à gérer plus tard avec le processus affichage
        directions dir = ask_user_dir();
        //Envoyer les infos avec un pipe
        // solution temp pour tester
        switch (dir) {
            case Up :
                end_game = 0;
                break;
            case Right :
                end_game = 1;
                break;
            case Down :
                end_game = 2;
                break;
            case Left :
                end_game = 3;
                break;
        }
    }
    switch (end_game) {
        case 0 :
            printf("Keske tu fous là ?\n");
            break;
        case 1 :
            printf("GG\n");
            break;
        case 2 :
            printf("T'as perdu le jeu\n");
            break;
        default :
            printf("Wtf\n");
            break;
    }
    
    desinit_grid(&grid);

    return 0;
}
