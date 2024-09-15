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
void print_home_msg(void);

// Executing Arguments
int execute_args(char **args);

// New processes
int new_process(char **args);

// built in functions
int own_cd(char **args);
int own_help(char **args);
int own_exit(char **args);

// colors :D

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define MAGENTA "\033[1;35m"

void setColor(const char *);
void resetColor();

#endif
