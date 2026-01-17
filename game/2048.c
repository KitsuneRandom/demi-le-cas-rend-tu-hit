#define _POSIX_C_SOURCE 200112L
#include "2048.h"
#include "../display/display.h"

grid g;
pid_t self_pid;
pid_t display_pid;
pid_t main_pid;
bool run = false;

void end_game() {
    run = false;
    kill(main_pid, SIGRTMIN + 9);
}

void* thread_move_score(void* arg) {
    sigset_t set;
    sigemptyset(&set);

    for (int i = 1; i <= 4; i++) sigaddset(&set, SIGRTMIN + i);
    sigaddset(&set, SIGRTMIN + 9);

    while (run) {
        int signum;
        sigwait(&set, &signum);

        if (signum != SIGRTMIN + 9) {
            if (signum == SIGRTMIN + 1) {
                move_all(&g, Up);
            } else if (signum == SIGRTMIN + 2) {
                move_all(&g, Left);
            } else if (signum == SIGRTMIN + 3) {
                move_all(&g, Down);
            } else if (signum == SIGRTMIN + 4) {
                move_all(&g, Right);
            }
            add_random_cell(&g);

            kill(self_pid, SIGRTMIN + 5);
        }
    }

    pthread_exit(NULL);
}

void* thread_goal(void* arg) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    display_pid = fork();

    if (display_pid) {
        // parent
        close(pipefd[0]);
        write(pipefd[1], &g, sizeof(grid));

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN + 5);

        int signum;

        game_state state;

        while (run) {
            sigwait(&set, &signum);
            state = check_game_state(&g);
            
            if (state != ONGOING) end_game();

            write(pipefd[1], &g, sizeof(grid));
            write(pipefd[1], &state, sizeof(game_state));
            kill(self_pid, SIGRTMIN + 6);
        }
        close(pipefd[1]);
        wait(NULL);
    }
    else {
        // child
        close(pipefd[1]);

        grid received_grid;
        game_state state;

        read(pipefd[0], &received_grid, sizeof(grid));
        printf("Score : %d\n\n", received_grid.score);
        print_grid(received_grid.cells);
        kill(self_pid, SIGRTMIN + 8);

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN + 7);

        int signum;

        while (run) {
            sigwait(&set, &signum);

            system("clear");
            read(pipefd[0], &received_grid, sizeof(grid));
            read(pipefd[0], &state, sizeof(game_state));

            if (state != ONGOING) end_game(); // child and parent don't share run variable

            if (run) {
                printf("Score : %d\n\n", received_grid.score);
                print_grid(received_grid.cells);

                if (state == WIN) printf("Félicitations ! Vous avez gagné !\n");
                else if (state == LOSE) printf("Dommage ! Vous avez perdu !\n");
            }

            kill(self_pid, SIGRTMIN + 8);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }

    pthread_exit(NULL);
}

void* thread_main(void* arg) {

    pthread_t move_score;
    pthread_t goal;

    pthread_create(&move_score, NULL, thread_move_score, NULL);
    pthread_create(&goal, NULL, thread_goal, NULL);

    int o = open("main_to_main", O_RDONLY);
    if (o == -1) {
        perror("open main_to_main");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = read(o, &main_pid, sizeof(main_pid));
    if (bytes_read == -1) {
        perror("read pid from main_to_main");
        exit(EXIT_FAILURE);
    }

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN + 6);
    sigaddset(&set, SIGRTMIN + 8);
    
    int signum;
    
    directions direction;

    sigwait(&set, &signum);

    while (run) {
        kill(main_pid, SIGRTMIN);
        read(o, &direction, sizeof(direction));
        switch (direction) {
            case Up :
                // Move Up
                kill(self_pid, SIGRTMIN + 1);
                break;
            case Left :
                // Move Left
                kill(self_pid, SIGRTMIN + 2);
                break;
            case Down :
                // Move Down
                kill(self_pid, SIGRTMIN + 3);
                break;
            case Right :
                // Move Right
                kill(self_pid, SIGRTMIN + 4);
                break;
            default :
                break;
        }
        sigwait(&set, &signum);
        kill(display_pid, SIGRTMIN + 7);
        sigwait(&set, &signum);
    }
    pthread_kill(move_score, SIGRTMIN + 9);
    pthread_join(move_score, NULL);
    pthread_join(goal, NULL);

    close(o);
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    run = true;

    sigset_t mask;
    sigemptyset(&mask);
    for (int i = SIGRTMIN; i <= SIGRTMIN + 9; i++) sigaddset(&mask, i);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    g = (grid){0};
    add_random_cell(&g);

    self_pid = getpid();

    pthread_t main;
    pthread_create(&main, NULL, thread_main, NULL);
    pthread_join(main, NULL);

    print_grid(g.cells);

    exit(EXIT_SUCCESS);
}