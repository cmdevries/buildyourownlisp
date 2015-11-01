#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

#include "mpc.h"

int main(int argc, char** argv) {
    /* Create some basic parsers */
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* CLispy = mpc_new("clispy");

    /* Initialize a grammar for Reverse Polish Notation */
    mpca_lang(MPCA_LANG_DEFAULT,
    " \
        number : /-?[0-9]+/ ; \
        operator : '+' | '-' | '*' | '/' ; \
        expr : <number> | '(' <operator> <expr>+ ')' ; \
        clispy : /^/ <operator> <expr>+ /$/ ; \
    ",
    Number, Operator, Expr, CLispy);
    

    /* Print version infromation */
    puts("CLispy Version 0.0.2");
    puts("Press Ctrl+C to Exit\n");

    /* Forever */
    for (;;) {
        /* Display a prompt and read user input */
        char* input = readline("clispy> ");

        /* Add user input to history */
        add_history(input);

        /* Try to parse user input */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, CLispy, &r)) {
            /* On success print the AST */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            /* Print the error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        
        /* Deallocate input */
        free(input);
    }

    /* Undefined and delete parsers */
    mpc_cleanup(4, Number, Operator, Expr, CLispy);

    return 0;
}
