#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "game/game_feature.h"

int o = -1;
msg m;
sem_t* sem;

char ask_user_input(void) {
    char input;
    bool is_ok = false;
    char c;
    while (is_ok != true && m.run) {
        printf("Entrez z (haut), q (gauche), s (bas) ou d (droite) : ");
        if (scanf(" %c", &input) != 1) {
            if (!m.run) return 0;
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

// Fonction permettant de terminer la partie en cours
void end_game(int sig) {
    m.run = false;
    kill(getpid(), SIGRTMIN + 9);
}

int main() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    sigaddset(&set, SIGRTMIN + 9);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // Handler pour terminer proprement
    struct sigaction sa;
    sa.sa_handler = end_game;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);

    // Définition d'un message de création de partie
    m.pid = getpid();
    strncpy(m.tty, ttyname(STDIN_FILENO), sizeof(m.tty) - 1);       // ttyname(STDIN_FILENO) retourne le nom du terminal associé à l'entrée standard
    m.new_game = true;
    m.dir = -1;

    o = open("main_to_main", O_WRONLY);
    if (o == -1) {
        perror("open main_to_main");
        exit(EXIT_FAILURE);
    }
    
    sem = sem_open("/sem2048", 0);
    sem_wait(sem);                                                  // Sécurisation de l'écriture dans le pipe partagé entre les mains

    ssize_t bytes_written = write(o, &m, sizeof(msg));
    if (bytes_written == -1) {
        perror("write msg to main_to_main");
        exit(EXIT_FAILURE);
    }

    m.run = true;
    m.new_game = false;

    int signum;

    // Boucle de jeu
    while (m.run) {
        sigwait(&set, &signum);                                     // Attente des threads de jeu
        if (signum != SIGRTMIN + 9) {
            m.dir = ask_user_dir();
            if (m.dir != WRONG) {
                sem_wait(sem);                                      // Sécurisation de l'écriture dans le pipe partagé entre les mains
                ssize_t bytes_written = write(o, &m, sizeof(msg));
                if (bytes_written == -1) {
                    perror("write msg to main_to_main");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            m.run = false;
        }
    }
    
    close(o);
    exit(EXIT_SUCCESS);
}
