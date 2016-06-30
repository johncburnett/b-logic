/* b-logic/src/b-parser.y - John Burnett (c) 2016 */

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
