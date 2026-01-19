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
pid_t child_pid;

char ask_user_input(void) {
    char input;
    bool is_ok = false;
    char c;
    while (is_ok != true && run) {
        printf("Entrez z (haut), q (gauche), s (bas) ou d (droite) : ");
        if (scanf(" %c", &input) != 1) {
            if (!run) return 0;
            continue;
        }
        
        while ((c = getchar()) != '\n' && c != EOF); // Vider le reste de la saisie de l'utilisateur après le premier caractère lu
        
        if (input != 'z' && input != 'q' && input != 's' && input != 'd') {
            printf("valeur saisie invalide.\n");
        } else {
            is_ok = true;
        }
    }
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
            return -1;
    }
}

void end_game() {
    run = false;
    kill(child_pid, SIGRTMIN + 9);
    kill(getpid(), SIGRTMIN + 9);
}

int main() {
    system("clear");
    child_pid = fork();
    if (child_pid) {
        // parent
        run = true;

        struct sigaction sa;
        sa.sa_handler = end_game;
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);
        sigaddset(&set, SIGRTMIN + 9);
        sigprocmask(SIG_BLOCK, &set, NULL);

        // parent
        int main_to_main = mkfifo("main_to_main", 0666);
        if (main_to_main == -1) {
            end_game();
            perror("mkfifo main_to_main");
            exit(EXIT_FAILURE);
        }

        int o = open("main_to_main", O_WRONLY);
        if (o == -1) {
            end_game();
            perror("open main_to_main");
            exit(EXIT_FAILURE);
        }

        pid_t pid = getpid();
        ssize_t bytes_written = write(o, &pid, sizeof(pid));
        if (bytes_written == -1) {
            end_game();
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
                    end_game();
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
