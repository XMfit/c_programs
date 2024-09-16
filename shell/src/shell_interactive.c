#include "../include/shell.h"

char **my_completion(const char *text, int start, int end);
char *my_generator(const char *text, int state);

void shell_interactive (void) {

    char *args[COMMAND_ARGS];
    char prompt[BUFFER];

    // readline() is interactive and gets weird with setColor funcs
    // so im just manually adding the colors to the string
    snprintf(prompt, sizeof(prompt), "\001\033[1;35m\002%s\001\033[0m\002 ", PROMPT);

    // auto-completion function
    rl_attempted_completion_function = my_completion;
    int status = 0;

    print_home_msg();

    while (!status) {
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

void print_home_msg() {
    setColor(GREEN);
    printf("\n\n");
    printf("               /$$           /$$                 /$$                 /$$ /$$\n");
    printf("              |__/          |__/                | $$                | $$| $$\n");
    printf(" /$$$$$$/$$$$  /$$ /$$$$$$$  /$$        /$$$$$$$| $$$$$$$   /$$$$$$ | $$| $$\n");
    printf("| $$_  $$_  $$| $$| $$__  $$| $$       /$$_____/| $$__  $$ /$$__  $$| $$| $$\n");
    printf("| $$ \\ $$ \\ $$| $$| $$  \\ $$| $$      |  $$$$$$ | $$  \\ $$| $$$$$$$$| $$| $$\n");
    printf("| $$ | $$ | $$| $$| $$  | $$| $$       \\____  $$| $$  | $$| $$_____/| $$| $$\n");
    printf("| $$ | $$ | $$| $$| $$  | $$| $$       /$$$$$$$/| $$  | $$|  $$$$$$$| $$| $$\n");
    printf("|__/ |__/ |__/|__/|__/  |__/|__/      |_______/ |__/  |__/ \\_______/|__/|__/\n");
    resetColor();
    printf("\n\n");
}

// Auto completion function used by readline
char **my_completion(const char *text, int start, int end) {
    rl_completion_append_character = ' ';
    return rl_completion_matches(text, my_generator);
}

// The generator function called by readline to generate possible matches
char *my_generator(const char *text, int state) {
    // Current cmd list (need to figure out how to add more automatically during runtime)
    static const char *commands[] = {"cd", "env", "help", "exit", "ls", "clear", "gcc", "git", "vim", NULL};
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
