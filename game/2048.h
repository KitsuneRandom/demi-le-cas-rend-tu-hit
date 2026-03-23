#pragma once

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "game_feature.h"

#define MAX_GAMES 32   // nombre maximum de parties simultanées

// structure envoyée dans le pipe nommé
// Elle permet d'identifier quel processus main envoie la commande
typedef struct message {
    pid_t pid;        // pid du processus main
    directions dir;   // direction demandée
    bool new_game;    // true si création d'une nouvelle partie
} message;


// structure stockant une partie
typedef struct {
    grid g;
    pid_t main_pid;
    pthread_t move_score_thread;   // MODIFICATION
    pthread_t goal_thread;         // MODIFICATION
} game;


// Threads du processus 2048
void* thread_move_score(void* arg);
void* thread_goal(void* arg);
void* thread_main(void* arg);