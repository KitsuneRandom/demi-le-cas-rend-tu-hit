#define _POSIX_C_SOURCE 200112L
#include "2048.h"
#include "../display/display.h"

uint16_t* grid;
bool run = false;

void end_game() {run = false;}

void* thread_move_score(void* arg) {
    printf("thread move score\n");
    while (run) {
        // Move and score logic here
    }

    pthread_exit(NULL);
}

void* thread_goal(void* arg) {
    printf("thread goal\n");
    while (run) {
        // Goal checking logic here
    }

    pthread_exit(NULL);
}

void* thread_main(void* arg) {
    printf("thread main\n");
    pthread_t move_score;
    pthread_t goal;

    pthread_create(&move_score, NULL, thread_move_score, NULL);
    pthread_create(&goal, NULL, thread_goal, NULL);

    while (run) {
        // Game loop logic here
    }

    pthread_join(move_score, NULL);
    pthread_join(goal, NULL);

    pthread_exit(NULL);
}

int main() {
    printf("run game\n");
    init_grid(&grid);
    add_random_cell(grid);
    run = true;

    struct sigaction sa_end;
    sa_end.sa_handler = end_game;
    sa_end.sa_flags = 0;
    sa_end.sa_mask = (sigset_t){0};
    sigaction(SIGINT, &sa_end, NULL);

    pthread_t main;
    pthread_create(&main, NULL, thread_main, NULL);
    pthread_join(main, NULL);

    print_grid(grid);

    desinit_grid(&grid);
    return EXIT_SUCCESS;
}