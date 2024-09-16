#include "../include/shell.h"

int new_process(char **args) {
    // Pipe check
    int pipe_index = -1, args_length = 0;
    while (args[args_length] != NULL)
        args_length++;

    // Find pipe index
    for (int i = 0; i < args_length; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    // Run with pipe
    if (pipe_index != -1) {

        // Build cmd1 & cmd2
        char *cmd1[pipe_index + 1];
        char *cmd2[args_length - pipe_index];

        // Build cmd1
        for (int i = 0; i < pipe_index; i++) {
            cmd1[i] = args[i];
        }
        cmd1[pipe_index] = NULL;  // null terminator

        // Build cmd2
        for (int i = 0; i < args_length - pipe_index - 1; i++) {
            cmd2[i] = args[pipe_index + 1 + i];
        }
        cmd2[args_length - pipe_index - 1] = NULL;  // null terminator

        /* Debugg statements
        printf("cmd1: %s\n", cmd1[0]);
        for (int i = 0; cmd1[i] != NULL; i++) {
            printf("cmd1 arg[%d]: %s\n", i, cmd1[i]);
        }

        printf("cmd2: %s\n", cmd2[0]);
        for (int i = 0; cmd2[i] != NULL; i++) {
            printf("cmd2 arg[%d]: %s\n", i, cmd2[i]);
        }
        */

        // Create pipe
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("Error creating pipe");
            return 0;
        }

        // fork for cmd1
        pid_t pid1 = fork();
        if (pid1 == 0) {  // child process for cmd1
            close(pipefd[0]);               // close read end
            dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
            close(pipefd[1]);               // close write end after redirect
            execvp(cmd1[0], cmd1);          // execute cmd1
            fflush(stdout);                 // idk if this really does anything
            perror("execvp failed");
            exit(1);       
        }

        // fork for cmd2
        pid_t pid2 = fork();
        if (pid2 == 0) {  // child process for cmd2
            close(pipefd[1]);               // close write end
            dup2(pipefd[0], STDIN_FILENO);  // redirect stdin from pipe
            close(pipefd[0]);               // close read end after redirect
            execvp(cmd2[0], cmd2);          // execute cmd2
            perror("execvp failed");
            exit(1);
        }

        // Close pipe in parent
        close(pipefd[0]);
        close(pipefd[1]);

        // Wait for both children to finish
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);


    } else {
        // Run single command
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("fork failed");
        }
    }

    return 0;
}
