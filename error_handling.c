#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

#include "mpc.h"

/* Used for returning multiple types and errors */
typedef struct {
    int type;
    long num;
    int err;
} lval;

/* Possible lval types */
enum { LVAL_NUM, LVAL_ERR };

/* Possible error types */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* Create number type lval */
lval lval_num(long x) {
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}

/* Create error type lval */
lval lval_err(int x) {
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

/* Print an lval */
void lval_print(lval v) {
    switch (v.type) {
        case LVAL_NUM: printf("%li", v.num); break;
        case LVAL_ERR:
            if (v.err == LERR_DIV_ZERO) printf("Error: Divide By Zero!");
            if (v.err == LERR_BAD_OP) printf("Error: Invalid Operator!");
            if (v.err == LERR_BAD_NUM) printf("Error: Invalid Number!");
            break;
    }
}

/* Print lval then newline */
void lval_println(lval v) { lval_print(v); putchar('\n'); }

/* Switch on operator and combine x and y that way */
lval lval_op(lval x, char* op, lval y) {
    /* If already error, return it */
    if (x.type == LVAL_ERR) { return x; }
    if (y.type == LVAL_ERR) { return y; }

    if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
    if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
    if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
    if (strcmp(op, "/") == 0) { 
        return y.num == 0
            ? lval_err(LERR_DIV_ZERO)
            : lval_num(x.num / y.num);
    }

    return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {
    /* Base case: if it is a number return it directly */
    if (strstr(t->tag, "number")) {
        /* Check for error in conversion */
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
    }

    char* op = t->children[1]->contents;
    lval x = eval(t->children[2]);

    int i = 3;
    while (strstr(t->children[i]->tag, "expr")) {
        x = lval_op(x, op, eval(t->children[i]));
        i++;
    }

    return x;
}

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
    puts("CLispy Version 0.0.3");
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
            lval result = eval(r.output);
            lval_println(result);
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
