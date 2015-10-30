#include <stdio.h>

#define MAX_INPUT 2048

/* Input buffer for keyboard input from user */
static char input[MAX_INPUT];

int main(int argc, char** argv) {
    /* Print version infromation */
    puts("CLispy Version 0.0.1");
    puts("Press Ctrl+C to Exit\n");

    /* Forever */
    for (;;) {
        /* Display a prompt */
        fputs("clispy> ", stdout);

        /* Read user input */
        fgets(input, MAX_INPUT, stdin);

        /* Echo input back to user */
        printf("No you're a %s", input);
    }

    return 0;
}
