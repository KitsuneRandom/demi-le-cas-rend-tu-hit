#define _POSIX_C_SOURCE 200112L
#include "2048.h"
#include "../display/display.h"

game games[MAX_GAMES];
int game_count = 0;
int id;                 // Stocke l'id de la game concernée durant l'exécution
// id n'est pas du tout protégée, il faudra mettre un mutex je suppose

pid_t self_pid;
pid_t display_pid;

pthread_t move_score;
pthread_t goal;
pthread_t main_thread;

bool run = false;

// Fonction pour retrouver la partie associée à un main
int find_game(pid_t pid) {
    for (int i = 0; i < game_count; i++) {
        if (games[i].main_pid == pid)
            return i;
    }
    return -1;
}

void end_game() {
    run = false;
    kill(games[id].main_pid, SIGRTMIN + 9);
    pthread_kill(move_score, SIGRTMIN + 9);
    pthread_kill(goal, SIGRTMIN + 9);
    pthread_kill(main_thread, SIGRTMIN + 9);
}

void* thread_move_score(void* arg) {
    sigset_t set;
    sigemptyset(&set);

    for (int i = SIGRTMIN + 1; i <= SIGRTMIN + 4; i++) sigaddset(&set, i);
    sigaddset(&set, SIGRTMIN + 9);

    int signum;

    while (run) {
        sigwait(&set, &signum);

        if (signum != SIGRTMIN + 9) {
            if (signum == SIGRTMIN + 1) {
                move_all(&games[id].g, Up);
            } else if (signum == SIGRTMIN + 2) {
                move_all(&games[id].g, Left);
            } else if (signum == SIGRTMIN + 3) {
                move_all(&games[id].g, Down);
            } else if (signum == SIGRTMIN + 4) {
                move_all(&games[id].g, Right);
            }
            add_random_cell(&games[id].g);

            pthread_kill(goal, SIGRTMIN);
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
        write(pipefd[1], &games[id].g, sizeof(grid));

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);
        sigaddset(&set, SIGRTMIN + 9);

        int signum;

        game_state state;

        while (run) {
            sigwait(&set, &signum);

            state = check_game_state(&games[id].g);

            write(pipefd[1], &games[id].g, sizeof(grid));
            if (signum == SIGRTMIN + 9) state = LOSE;
            write(pipefd[1], &state, sizeof(game_state));
            pthread_kill(main_thread, SIGRTMIN);

            if (state != ONGOING) end_game();
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
        kill(self_pid, SIGRTMIN + 5);

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);

        int signum;

        while (run) {
            sigwait(&set, &signum);

            read(pipefd[0], &received_grid, sizeof(grid));
            read(pipefd[0], &state, sizeof(game_state));

            system("clear");
            printf("Score : %d\n\n", received_grid.score);
            print_grid(received_grid.cells);
            
            if (state == WIN) printf("Félicitations ! Vous avez gagné !\n");
            else if (state == LOSE) printf("Dommage ! Vous avez perdu !\n");
            
            if (state != ONGOING) end_game(); // child and parent don't share run variable
            else kill(self_pid, SIGRTMIN + 5);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }

    pthread_exit(NULL);
}

void* thread_main(void* arg) {
    pthread_create(&move_score, NULL, thread_move_score, NULL);
    pthread_create(&goal, NULL, thread_goal, NULL);

    int o = open("main_to_main", O_RDONLY);
    if (o == -1) {
        end_game();
        perror("open main_to_main");
        exit(EXIT_FAILURE);
    }

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    sigaddset(&set, SIGRTMIN + 5);
    sigaddset(&set, SIGRTMIN + 9);
    
    int signum;
    
    msg msg_received;

    while (run) {
        sigwait(&set, &signum);
        if (signum != SIGRTMIN + 9) {
            if (read(o, &msg_received, sizeof(msg)) == -1) {
                end_game();
                perror("read msg from main_to_main");
                exit(EXIT_FAILURE);
            }
            if (msg_received.new_game) {
                id = game_count;
                games[id].main_pid = msg_received.pid;
                games[id].g = (grid){0};
                
                add_random_cell(&games[id].g);
                
                game_count++;
            } else {
                id = find_game(msg_received.pid);
                switch (msg_received.dir) {
                    case Up :
                        // Move Up
                        pthread_kill(move_score, SIGRTMIN + 1);
                        break;
                    case Left :
                        // Move Left
                        pthread_kill(move_score, SIGRTMIN + 2);
                        break;
                    case Down :
                        // Move Down
                        pthread_kill(move_score, SIGRTMIN + 3);
                        break;
                    case Right :
                        // Move Right
                        pthread_kill(move_score, SIGRTMIN + 4);
                        break;
                    default :
                        end_game();
                        break;
                }
            }
            kill(games[id].main_pid, SIGRTMIN);
            sigwait(&set, &signum);
            if (signum != SIGRTMIN + 9) {
                kill(display_pid, SIGRTMIN);
                sigwait(&set, &signum);
            }
        }
    }
    
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
    for (int i = SIGRTMIN; i <= SIGRTMIN + 5; i++) sigaddset(&mask, i);
    sigaddset(&mask, SIGRTMIN + 9);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    self_pid = getpid();

    pthread_create(&main_thread, NULL, thread_main, NULL);
    pthread_join(main_thread, NULL);

    exit(EXIT_SUCCESS);
}
