#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(void) {
    if (fork()) {
        // parent
        signal(SIGINT, SIG_IGN);
        wait(NULL);
    }
    else {
        // enfant
        execv("./game/2048", (char*[]){"./2048", NULL});
    }

    return EXIT_SUCCESS;
}
