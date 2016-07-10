/**
 * b-logic/src/and_xor.c - John Burnett (c) 2016
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char *argv[]) {
    num_tokens = 0;

    // set yyin to file if passed by user
    if(argc > 1) {
        yyin = fopen(argv[1], "r");
        if(yyin == NULL) {
            printf("error: invalid file\n");
            exit(1);
        }
    }

    printf("> ");
    yyparse();
    generate_pla(root);
    and_or_not(root);
    free_ast(root);

    return 0;
}
