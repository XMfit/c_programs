#include "../include/shell.h"

/* Piping Funcs */
void execute_pipes(char ***commands, int num_commands) {
    int pipefd[2];
    pid_t pid;
    int fd_in = 0;  // input for the next command

    for (int i = 0; i < num_commands; i++) {
        if (pipe(pipefd) == -1) {
            perror("pipe failed");
        }

        if ((pid = fork()) == -1) {
            perror("fork failed");
        }

        if (pid == 0) {
            // in child process
            dup2(fd_in, STDIN_FILENO);  // get input from the previous command
            if (i < num_commands - 1) {
                dup2(pipefd[1], STDOUT_FILENO); // redirect output to the pipe for the next command
            }
            close(pipefd[0]);  // close unused read end
            close(pipefd[1]);  // close write end after redirect

            // execute the current command
            execvp(commands[i][0], commands[i]);
            perror("execvp failed");
        } else {
            // in parent process
            wait(NULL);          // wait for the child to finish
            close(pipefd[1]);    // close write end in the parent
            fd_in = pipefd[0];   // use read end as input for the next command
        }
    }

    // idk if i need this code
    close(pipefd[0]);
    close(pipefd[1]);
}

void parse_args_into_commands(char *args[], char ***commands, int *num_commands) {
    *num_commands = 0;
    int command_start = 0;
    int arg_index = 0;
    
    while (args[arg_index] != NULL) {
        if (strcmp(args[arg_index], "|") == 0) {
            // Create a command between command_start and arg_index
            int command_length = arg_index - command_start;
            commands[*num_commands] = malloc((command_length + 1) * sizeof(char *));
            
            for (int i = 0; i < command_length; i++) {
                commands[*num_commands][i] = args[command_start + i];
            }
            commands[*num_commands][command_length] = NULL;
            
            (*num_commands)++;
            command_start = arg_index + 1; // start the next command after the pipe
        }
        arg_index++;
    }
    
    // Add the final command post last pipe
    int command_length = arg_index - command_start;
    commands[*num_commands] = malloc((command_length + 1) * sizeof(char *));
    for (int i = 0; i < command_length; i++) {
        commands[*num_commands][i] = args[command_start + i];
    }
    commands[*num_commands][command_length] = NULL;
    (*num_commands)++;
}

/* Output Redirection Funcs */

/* Input Redirection Funcs */

int new_process(char **args) {
    // get length of **args
    int args_length = 0;
    while (args[args_length] != NULL)
        args_length++;


    // find if pipe is in cmd
    int has_pipe = 0;
    for (int i = 0; i < args_length; i++) {
        if (strcmp(args[i], "|") == 0) {
            has_pipe;
            break;
        }
    }

    if (has_pipe) {
        // 2d char *pointer array
        char **commands[MAX_COMMANDS];
        int num_commands = 0;

        // fill in commands, and execute
        parse_args_into_commands(args, commands, &num_commands);
        execute_pipes(commands, num_commands);

        // free up commands
        for (int i = 0; i < num_commands; i++) {
            free(commands[i]);
        }

    } else {
        // Run single command
        pid_t pid;

        if ((pid = fork()) == -1) {
            perror("fork failed");
        }

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
