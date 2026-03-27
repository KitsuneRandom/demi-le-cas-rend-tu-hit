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

typedef struct game {
    pid_t main_pid;
    char tty[128];
    grid grid;
    bool run;
} game;

void* thread_move_score(void* arg);
void* thread_goal(void* arg);
void* thread_main(void* arg);
