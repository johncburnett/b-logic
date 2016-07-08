/*
 * b-logic/src/b-parser.y
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
 *
 * Bison parser for loading digital logic expressions into an AST
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
%}

%union {
    struct ast *a;
    char s[4];
}

/* tokens */
%token <s> STRING
%token EOL
%type <a> exp factor term

%%
calclist: /* nothing */
| calclist exp EOL {
    /*traverse($2);*/
    generate_pla($2);
    and_or_not($2);
    free_ast($2); /* free ast */
    printf("> ");
}
| calclist EOL { printf("> "); } /* blank line or a comment */
;

exp: factor
| exp '+' factor { $$ = new_ast('+', $1,$3); }
;

factor: term
| factor '*' term { $$ = new_ast('*', $1,$3); }
| factor '^' term { $$ = new_ast('^', $1,$3); }
;

term: STRING {
    int len = strlen(yyval.s);
    char *b = (char *)malloc(sizeof(char) * (len+1));
    for(int i = 0; i < len; i++) {
        b[i] = yyval.s[i];
    }
    b[len] = '\0';
    $$ = new_node(&b);
}
| '!' term { $$ = new_ast('!', $2, NULL); }
| '|' term { $$ = new_ast('|', $2, NULL); }
| '(' exp ')' { $$ = $2; }
;
%%
