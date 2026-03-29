#pragma once

#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "game_feature.h"

#define MAX_GAMES 128   // nombre maximum de parties simultanées

// Structure qui stocke les informations d'une partie
typedef struct game {
    pid_t main_pid;         // PID du main qui gère la partie
    char tty[128];          // nom du terminal associé au main
    grid grid;              // grille de jeu
    bool run;               // indique si la partie est en cours ou terminée
} game;

void* thread_move_score(void* arg);
void* thread_goal(void* arg);
void* thread_main(void* arg);
