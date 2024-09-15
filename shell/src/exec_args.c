#include "../include/shell.h"

int execute_args(char **args) {
    // built in func list to simulate built in shell code
    char *builtin_func_list[] = {
        "cd",
        "history",
        "help",
        "exit"
    };

    // array of function pointers for built in functions
    int (*builtin_func[])(char **) = {
        &own_cd,
        &own_history,
        &own_help,
        &own_exit
    };

    if (args[0] != NULL) {
        // Check if the command matches a built-in function
        write_to_history(args);
        for (int i = 0; i < sizeof(builtin_func_list) / sizeof(char *); i++) {
            if (strcmp(args[0], builtin_func_list[i]) == 0) {
                return ((*builtin_func[i])(args));
            }
        }
        return new_process(args);
    }

    return 1;
}
