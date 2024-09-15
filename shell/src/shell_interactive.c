#include "../include/shell.h"

void shell_interactive (void) {

    char command[COMMAND_BUFFER];
    char *args[COMMAND_ARGS];
    int status = 1;

    while (status) {
        printf("%s ", PROMPT);

        // Read input
        fgets(command, COMMAND_BUFFER, stdin);
        command[strcspn(command, "\n")] = '\0';

        // Parse input and store into args[]
        char *tokens = strtok(command, " ");
        int i = 0;
        while (tokens != NULL && i != COMMAND_ARGS) {
            args[i++] = tokens;
            tokens = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Execute commands
        status = execute_args(args);
    }
}
