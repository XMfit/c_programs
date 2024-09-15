#include "../include/shell.h"

int own_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "expected arguments: \"cd <dir-name>\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }

    return 1;
}

int own_history(char **args) {

    FILE *file = fopen(".mini_history", "r");
    if (file == NULL)
        perror("Error opening file");

    fseek(file, 0, SEEK_END);

    long file_size = ftell(file);
    int newline_count = 0;
    char buffer[BUFFER];
    long position;

    // Loop to count newlines and set pos to the start of the last N lines
    for (position = file_size - 1; position >= 0; position--) {
        fseek(file, position, SEEK_SET);
        char c = fgetc(file);
        if (c == '\n') {
            newline_count++;
            if (newline_count > 50)
                break;
        }
    }

    fseek(file, position + 1, SEEK_SET);

    while (fgets(buffer, BUFFER, file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
    
    return 1;
}

int write_to_history(char **args) {
    FILE *file = fopen(".mini_history", "a");
    if (file == NULL)
        perror("Error opening file");

    int i = 0;
    while (args[i] != NULL) {
        fprintf(file, "%s", args[i++]);
        if (args[i] != NULL) {
            fprintf(file, " ");
        }
    }

    fprintf(file, "\n");
    fclose(file);

    return 1;
}

int own_help(char **args) {
    printf("My simple shell\n");
    printf("Following built in commands:\n");
    printf("cd\n");
    printf("help\n");
    printf("exit\n");
    printf("Use man command for more info on commands\n");
    return 1;
}

int own_exit(char **args) {
    printf("Exiting shell\n");
    return 0;
}
