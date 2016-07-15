/**
 * b-logic/src/and_xor.c
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
 *
 * Main function for generating and_xor executable. Converts logic expressions
 * from stdin or file to C2 Cannonical AND-XOR form.
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char *argv[]) {
    num_tokens = 0;
    num_vars = 0;
    num_minterms = 0;

    // set yyin to file if passed by user
    if(argc > 1) {
        yyin = fopen(argv[1], "r");
        if(yyin == NULL) {
            printf("error: invalid file\n");
            exit(1);
        }
    }
    else {
        printf("> "); // prompt
    }

    yyparse();
    fclose(yyin);
    generate_pla(root);

    and_or_not(root);
    freopen("/dev/tty", "a", stdout); // make sure stdout is restored

    free_ast(root);
    empty_tokens();
    reformat_output();
    remove("in.pla"); // cleanup

    // now we parse the SoP form of our input and minimize manually
    yyin = fopen("out.pla", "r");
    yyparse();
    fclose(yyin);

    generate_minterms(root);
    minterms_to_ascii();

    and_xor(root);

    free_ast(root);
    empty_tokens();

    // print output
    char *fname = (char *)"out.pla";
    print_file(&fname);

    return 0;
}
