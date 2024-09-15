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

int own_env(char **args) {
    for (char **env = __environ; *env != NULL; ++env) {
        printf("%s\n", *env);
    }
    return 1;
}

int own_exit(char **args) {
    printf("Exiting shell\n");
    return 0;
}

int own_help(char **args) {
    printf("My simple shell\n");
    printf("Following built in commands:\n");
    printf("cd\n");
    printf("env\n");
    printf("help\n");
    printf("exit\n");
    printf("Use man command for more info on commands\n");
    return 1;
}
