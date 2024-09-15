#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEARCH_BUFFER 1024
#define URL_SIZE 2048
#define COMMAND_SIZE 4096

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Incorrect amount of arguments\n");
        exit(EXIT_FAILURE);
    }

    char search_terms[SEARCH_BUFFER] = "";
    char url[URL_SIZE];
    char command[COMMAND_SIZE];

    // build search terms
    for (int i = 1; i < argc; i++) {
        strncat(search_terms, argv[i], sizeof(search_terms) - strlen(search_terms) - 1);
        if (i < argc - 1)
            strncat(search_terms, "+", sizeof(search_terms) - strlen(search_terms) - 1);
    }

    // build url
    snprintf(url, sizeof(url), "https://www.google.com/search?q=%s", search_terms);

    // Construct cmd to open browser
    snprintf(command, sizeof(command), "xdg-open \"%s\" 2>/dev/null", url);
    int result = system(command);

    if (result != 0) {
        fprintf(stderr, "Failed to open browser\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
