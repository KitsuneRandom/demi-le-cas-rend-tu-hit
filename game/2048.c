#include "2048.h"
#include "../display/display.h"
#include "game_feature.h"
#include <signal.h>

game games[MAX_GAMES];
size_t game_count = 0;
size_t current_id;

pid_t self_pid;
pid_t display_pid;

pthread_t move_score;
pthread_t goal;
pthread_t main_thread;

bool run = false;

// Fonction pour retrouver la partie associée à un main
size_t find_game(pid_t pid) {
    for (size_t i = 0; i < game_count; i++) {
        if (games[i].main_pid == pid)
            return i;
    }
    return -1;
}

void end_game_engine(int sig) {
    run = false;
    pthread_kill(move_score, SIGRTMIN + 9);
    pthread_kill(goal, SIGRTMIN + 9);
    pthread_kill(main_thread, SIGRTMIN + 9);
    kill(display_pid, SIGRTMIN + 9);
    remove("main_to_main");

    for (size_t i = 0; i < game_count; i++) {
        kill(games[i].main_pid, SIGINT);
    }
}

void* thread_move_score(void* arg) {
    sigset_t set;
    sigemptyset(&set);

    for (int i = SIGRTMIN; i <= SIGRTMIN + 4; i++) sigaddset(&set, i);
    sigaddset(&set, SIGRTMIN + 9);

    int signum;

    while (run) {
        sigwait(&set, &signum);

        if (signum != SIGRTMIN + 9) {
            if (signum == SIGRTMIN + 1) {
                move_all(&games[current_id].grid, Up);
            } else if (signum == SIGRTMIN + 2) {
                move_all(&games[current_id].grid, Left);
            } else if (signum == SIGRTMIN + 3) {
                move_all(&games[current_id].grid, Down);
            } else if (signum == SIGRTMIN + 4) {
                move_all(&games[current_id].grid, Right);
            }
            if (games[current_id].run) add_random_cell(&games[current_id].grid);

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

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);
        sigaddset(&set, SIGRTMIN + 9);

        int signum;

        display_msg dmsg;

        while (run) {
            sigwait(&set, &signum);

            dmsg.state = check_game_state(&games[current_id].grid);
            dmsg.grid = games[current_id].grid;
            strncpy(dmsg.tty, games[current_id].tty, sizeof(dmsg.tty) - 1);

            if (!games[current_id].run) dmsg.state = LOSE;
            if (signum == SIGRTMIN + 9) dmsg.state = DEAD;

            write(pipefd[1], &dmsg, sizeof(display_msg));
            pthread_kill(main_thread, SIGRTMIN);
            if (dmsg.state != ONGOING) {
                kill(games[current_id].main_pid, SIGRTMIN + 9);
            }
        }
        close(pipefd[1]);
        wait(NULL);
    }
    else {
        // child
        close(pipefd[1]);

        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);
        sigaddset(&set, SIGRTMIN + 9);
        
        int signum;
        display_msg dmsg;
        int fd_console;

        while (run) {
            sigwait(&set, &signum);

            if (signum != SIGRTMIN + 9) {

                read(pipefd[0], &dmsg, sizeof(display_msg));

                fd_console = open(dmsg.tty, O_WRONLY);

                dprintf(fd_console, "\033[H\033[J");
                dprintf(fd_console, "Score : %d\n\n", dmsg.grid.score);

                print_grid(dmsg.grid.cells, fd_console);
                
                if (dmsg.state == WIN) dprintf(fd_console, "Félicitations ! Vous avez gagné !\n");
                else if (dmsg.state == LOSE) dprintf(fd_console, "Dommage ! Vous avez perdu !\n");
                else if (dmsg.state == DEAD) {
                    dprintf(fd_console, "Le moteur de jeu est MORT !\n");
                    run = false;
                }
            }
            kill(self_pid, SIGRTMIN + 5);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }

    pthread_exit(NULL);
}


void* thread_main(void* arg) {
    pthread_create(&move_score, NULL, thread_move_score, NULL);
    pthread_create(&goal, NULL, thread_goal, NULL);

    int o = open("main_to_main", O_RDONLY | O_NONBLOCK);
    if (o == -1) {
        end_game_engine(0);
        perror("open main_to_main");
        exit(EXIT_FAILURE);
    }

    run = true;

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    sigaddset(&set, SIGRTMIN + 5);
    sigaddset(&set, SIGRTMIN + 9);

    int signum;
    msg msg_received;

    ssize_t bytes_read;

    struct pollfd pfd;
    pfd.fd = o;
    pfd.events = POLLIN;

    while (run) {
        if (signum != SIGRTMIN + 9) {
            int ret = poll(&pfd, 1, 100);

            if (ret == -1) {
                end_game_engine(0);
                perror("poll");
                exit(EXIT_FAILURE);
            } else if (ret == 0) {
                continue;
            }
            
            bytes_read = read(o, &msg_received, sizeof(msg));
            if (bytes_read == -1) {
                end_game_engine(0);
                perror("read from main_to_main");
                exit(EXIT_FAILURE);
            }

            if (bytes_read == 0) continue;

            if (msg_received.new_game) {
                if (game_count < MAX_GAMES) {
                    games[game_count].main_pid = msg_received.pid;
                    strncpy(games[game_count].tty, msg_received.tty, sizeof(msg_received.tty) - 1);
                    games[game_count].grid = (grid){0};
                    games[game_count].run = true;
                    game_count++;
                    pthread_kill(move_score, SIGRTMIN);
                }
            } else {
                current_id = find_game(msg_received.pid);
                if (current_id != (size_t)-1 && msg_received.run == true) {
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
                    case WRONG :
                        break;
                    }
                }
                else if (msg_received.run == false) {
                    games[current_id].run = false;
                    pthread_kill(move_score, SIGRTMIN);
                }
                else {
                    pthread_kill(move_score, SIGRTMIN);
                }
            }
            sigwait(&set, &signum);
            if (signum != SIGRTMIN + 9) {
                kill(display_pid, SIGRTMIN);
                sigwait(&set, &signum);
                kill(games[current_id].main_pid, SIGRTMIN);
                current_id = -1;
            }
        }
    }

    close(o);

    pthread_join(move_score, NULL);
    pthread_join(goal, NULL);

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

    struct sigaction sa;
    sa.sa_handler = end_game_engine;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    int main_to_main = mkfifo("main_to_main", 0666);
        if (main_to_main == -1) {
            end_game_engine(0);
            perror("mkfifo main_to_main");
            exit(EXIT_FAILURE);
        }

    self_pid = getpid();

    pthread_create(&main_thread, NULL, thread_main, NULL);
    pthread_join(main_thread, NULL);

    exit(EXIT_SUCCESS);
}
