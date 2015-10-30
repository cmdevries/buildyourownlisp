#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

int main(int argc, char** argv) {
    /* Print version infromation */
    puts("CLispy Version 0.0.1");
    puts("Press Ctrl+C to Exit\n");

    /* Forever */
    for (;;) {
        /* Display a prompt and read user input */
        char* input = readline("clispy> ");

        /* Add user input to history */
        add_history(input);

        /* Echo input back to user */
        printf("No you're a %s\n", input);
        
        /* Deallocate input */
        free(input);
    }

    return 0;
}
