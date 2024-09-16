#include "../include/shell.h"

int new_process(char **args) {

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp failed");
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork failed");
    }

    return 0;
}
