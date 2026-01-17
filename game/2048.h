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

void* thread_move_score(void* arg);
void* thread_goal(void* arg);
void* thread_main(void* arg);