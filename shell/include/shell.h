#ifndef SHELL_H
#define SHELL_H

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* GNU Libraries */
#include <readline/readline.h>
#include <readline/history.h>

/* Macros */
#define COMMAND_BUFFER 1024
#define COMMAND_ARGS 64
#define PROMPT "mini-shell #"

/* functions */

// An interactive Unix RPEL shell
void shell_interactive(void);

// Executing Arguments
int execute_args(char **args);

// New processes
int new_process(char **args);

// built in functions
int own_cd(char **args);
int own_help(char **args);
int own_exit(char **args);

#endif
