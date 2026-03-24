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

typedef struct msg {
    pid_t pid;
    bool new_game;
    directions dir;
} msg;

typedef struct game {
    pid_t main_pid;
    grid g;
} game;

void* thread_move_score(void* arg);
void* thread_goal(void* arg);
void* thread_main(void* arg);
