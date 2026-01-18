#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "display.h"
#include "game/game_feature.h"

bool run = false;

char ask_user_input(void) {
    char input;
    char is_ok = false;
    char c;
    while (is_ok != true) {
        printf("Entrez z (haut), q (gauche), s (bas) ou d (droite) : ");
        scanf(" %c", &input);
        
        while ((c = getchar()) != '\n' && c != EOF); // Vider le reste de la saisie de l'utilisateur après le premier caractère lu
        
        if (input != 'z' && input != 'q' && input != 's' && input != 'd') {
            printf("valeur saisie invalide.\n");
        } else {
            is_ok = true;
        }
    }
    printf("user input : %c\n", input);
    return input;
}

directions ask_user_dir(void) {
    char input = ask_user_input();
    switch (input) {
        case 'z' :
            return Up;
        case 'q' :
            return Left;
        case 's' :
            return Down;
        case 'd' :
            return Right;
        default :
            return Up;
    }
}

void end_game() {run = false;}

int main() {
    system("clear");
    if (fork()) {
        // parent

        run = true;

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);
        sigaddset(&set, SIGRTMIN + 9);
        sigprocmask(SIG_BLOCK, &set, NULL);

        // parent
        int main_to_main = mkfifo("main_to_main", 0666);
        if (main_to_main == -1) {
            perror("mkfifo main_to_main");
            exit(EXIT_FAILURE);
        }

        int o = open("main_to_main", O_WRONLY);
        if (o == -1) {
            perror("open main_to_main");
            exit(EXIT_FAILURE);
        }

        pid_t pid = getpid();
        ssize_t bytes_written = write(o, &pid, sizeof(pid));
        if (bytes_written == -1) {
            perror("write pid to main_to_main");
            exit(EXIT_FAILURE);
        }
        printf("Bienvenue dans ce super 2048 (super jsp mais en tt cas 2048)\nLe but du jeu c'est de pas perdre le jeu, et pour ça il faut éviter de remplir la grille\nVous allez donc devoir fusionner les cases de même valeur jusqu'à obtenir une case 2048\nVous utiliserez z, q, s, et d pour vous déplpacer en haut, à gauche, en bas et à droite\nVous pouvez quitter a tout moment avec ctrl-c (promis, l'arret est clean)\n");
        
        int signum;

        while (run) {
            sigwait(&set, &signum);
            if (signum != SIGRTMIN + 9) {
                directions direction = ask_user_dir();
                ssize_t bytes_written = write(o, &direction, sizeof(direction));
                if (bytes_written == -1) {
                    perror("write direction to main_to_main");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                end_game();
            }
        }

        close(o);
        wait(NULL);
        remove("main_to_main");
    }
    else {
        // child
        execv("./game/2048", (char*[]){"./game/2048", NULL});
    }

    exit(EXIT_SUCCESS);

}
