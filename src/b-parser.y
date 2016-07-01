/* b-logic/src/b-parser.y
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
 */

%{
# include <stdio.h>
# include <stdlib.h>
# include "main.h"
%}

%union {
    struct ast *a;
    int d;
}

/* declare tokens */
%token <d> NUMBER
%token EOL
%type <a> exp factor term

%%
calclist: /* nothing */
| calclist exp EOL {
    printf("= %1d\n", eval($2)); /* evaluate and print ast */
    /*traverse($2);*/
    treefree($2); /* free ast */
    printf("> ");
}
| calclist EOL { printf("> "); } /* blank line or a comment */
;

exp: factor
| exp '+' factor { $$ = newast('+', $1,$3); }
;

factor: term
| factor '*' term { $$ = newast('*', $1,$3); }
;

factor: term
| factor '^' term { $$ = newast('^', $1,$3); }
;

term: NUMBER { $$ = newnum($1); }
| '|' term { $$ = newast('|', $2, NULL); }
| '(' exp ')' { $$ = $2; }
| '-' term { $$ = newast('M', $2, NULL); }
;
%%
