#include "2048.h"

int* grid;

void* thread_main(void* arg) {
    pthread_exit(NULL);
}

void* thread_move_score(void* arg) {
    pthread_exit(NULL);
}

void* thread_goal(void* arg) {
    pthread_exit(NULL);
}

int main() {
    grid = malloc(sizeof(int) * 16);

    pthread_t main;
    pthread_t move_score;
    pthread_t goal;

    pthread_create(&main, NULL, thread_main, NULL);
    pthread_create(&move_score, NULL, thread_move_score, NULL);
    pthread_create(&goal, NULL, thread_goal, NULL);
    pthread_join(&main, NULL);
    pthread_join(&move_score, NULL);
    pthread_join(&goal, NULL);

    free(grid);
}