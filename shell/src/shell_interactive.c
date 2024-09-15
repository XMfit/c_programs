#include "../include/shell.h"

char **my_completion(const char *text, int start, int end);
char *my_generator(const char *text, int state);

void shell_interactive (void) {

    char *args[COMMAND_ARGS];
    char prompt[COMMAND_BUFFER];
    snprintf(prompt, sizeof(prompt), "%s ", PROMPT);

    // auto-completion function
    rl_attempted_completion_function = my_completion;
    int status = 1;

    while (status) {
        // Read input
        char *input = readline(prompt);
        if (input == NULL) {
            fprintf(stderr, "Error reading input");
            break;
        }

        // Parse input and store into args[]
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL && i < COMMAND_ARGS) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Add input to history to be used 
        add_history(input);

        // Execute commands
        status = execute_args(args);
        free(input);
    }
}

// Auto completion function used by readline
char **my_completion(const char *text, int start, int end) {
    rl_completion_append_character = ' ';
    return rl_completion_matches(text, my_generator);
}

// The generator function called by readline to generate possible matches
char *my_generator(const char *text, int state) {
    // Curr cmd list (need to figure out how to add more automatically during runtime)
    static const char *commands[] = {"cd", "env", "help", "exit", "ls", "clear", "gcc", "github", "vim", NULL};
    static int list_index, len;
    
    // Initialize the search
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    // Return each matching command
    while (commands[list_index]) {
        const char *command = commands[list_index++];
        if (strncmp(command, text, len) == 0) {
            return strdup(command);  // Return matching command
        }
    }

    // No more matches
    return NULL;
}
