#define _POSIX_C_SOURCE 200112L

#include "2048.h"
#include "../display/display.h"

game games[MAX_GAMES];   // tableau de parties
int game_count = 0;      // compteur de parties

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
    for (int i = 0; i < game_count; i++) {
        kill(games[i].main_pid, SIGRTMIN + 9);      // signal de fin de partie pour tous les mains actifs
    }
    pthread_kill(move_score, SIGRTMIN + 9);
    pthread_kill(goal, SIGRTMIN + 9);
    pthread_kill(main_thread, SIGRTMIN + 9);
    unlink("main_to_main"); // le fifo est supprimé par 2048
}

void* thread_move_score(void* arg) {
    int id = *(int*)arg;
    free(arg);
    arg = NULL;

    sigset_t set;
    sigemptyset(&set);

    for (int i = 1; i <= 4; i++) sigaddset(&set, SIGRTMIN + i);
    sigaddset(&set, SIGRTMIN + 9);

    int signum;

    while (run) {
        sigwait(&set, &signum);

        // Cette version simplifiée suppose qu'une seule partie est manipulée à la fois, ce qui ne posera plus problème après la mise en place de mutex
        if (signum == SIGRTMIN + 1)
            move_all(&games[id].g, Up);
        if (signum == SIGRTMIN + 2)
            move_all(&games[id].g, Left);
        if (signum == SIGRTMIN + 3)
            move_all(&games[id].g, Down);
        if (signum == SIGRTMIN + 4)
            move_all(&games[id].g, Right);
        add_random_cell(&games[id].g);

        kill(self_pid, SIGRTMIN + 5);
    }

    pthread_exit(NULL);
}

void* thread_goal(void* arg) {
    int id = *(int*)arg;
    free(arg);
    arg = NULL;

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    display_pid = fork();

    if (display_pid) {
        // parent
        close(pipefd[0]);

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN + 5);

        int signum;

        game_state state;

        while (run) {
            sigwait(&set, &signum);

            state = check_game_state(&games[id].g);

            write(pipefd[1], &games[id].g, sizeof(grid));
            if (signum == SIGRTMIN + 9) state = LOSE;
            write(pipefd[1], &state, sizeof(game_state));
            kill(self_pid, SIGRTMIN + 6);

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
        kill(self_pid, SIGRTMIN + 8);

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN + 7);

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
            else kill(self_pid, SIGRTMIN + 8);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }

    pthread_exit(NULL);
}

void* thread_main(void* arg) {

    int o = open("main_to_main", O_RDONLY);
    if (o == -1) {
        end_game();
        perror("open main_to_main");
        exit(EXIT_FAILURE);
    }

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN + 6);
    sigaddset(&set, SIGRTMIN + 8);
    sigaddset(&set, SIGRTMIN + 9);
    
    int signum;

    sigwait(&set, &signum);

    message msg;

    while (run) {
        if (signum != SIGRTMIN + 9) {
            read(o, &msg, sizeof(msg));
            if (msg.new_game) {
                // création d'une nouvelle partie si on reçoit un message de création de partie (avec les threads associés)
                games[game_count].main_pid = msg.pid;
                games[game_count].g = (grid){0};

                add_random_cell(&games[game_count].g);

                int *arg = malloc(sizeof(int));
                *arg = game_count;
                pthread_create(&games[game_count].move_score_thread,
                            NULL,
                            thread_move_score,
                            arg);
                pthread_create(&games[game_count].goal_thread,
                            NULL,
                            thread_goal,
                            arg);
                game_count++;
            } else {
                // sinon, on traite le message avec la direction
                int id = find_game(msg.pid);
                if (id == -1)
                    continue;

                switch (msg.dir) {
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
                        end_game();
                        break;
                }
                sigwait(&set, &signum);
                kill(display_pid, SIGRTMIN + 7);
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
    for (int i = SIGRTMIN; i <= SIGRTMIN + 9; i++) sigaddset(&mask, i);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    self_pid = getpid();

    pthread_create(&main_thread, NULL, thread_main, NULL);
    pthread_join(main_thread, NULL);

    exit(EXIT_SUCCESS);
}