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
            return WRONG;
    }
}


int main() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    sigaddset(&set, SIGRTMIN + 9);
    sigprocmask(SIG_BLOCK, &set, NULL);


    msg m;
    m.pid = getpid();
    m.tty = ttyname(STDIN_FILENO);
    m.new_game = true;
    m.dir = -1;

    int o = open("main_to_main", O_WRONLY);
    if (o == -1) {
        perror("open main_to_main");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(o, &m, sizeof(msg));
    if (bytes_written == -1) {
        perror("write pid to main_to_main");
        exit(EXIT_FAILURE);
    }

    run = true;

    int signum;

    while (run) {
        sigwait(&set, &signum);
        if (signum == SIGRTMIN) {
            m.new_game = false;
            m.dir = ask_user_dir();
            if (m.dir != WRONG) {
                ssize_t bytes_written = write(o, &m, sizeof(msg));
                if (bytes_written == -1) {
                    perror("write move to main_to_main");
                    exit(EXIT_FAILURE);
                }
            }
        } else if (signum == SIGRTMIN + 9) {
            run = false;
        }
    }
    
    exit(EXIT_SUCCESS);
}
