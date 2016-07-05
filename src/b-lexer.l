/* b-logic/src/b-lexer.l                                                */
/*                                                                      */
/* Copyright 2016 - John Bass, DMS Design                               */
/* Copyright 2016 - John Burnett, Developer                             */
/*                                                                      */
/* Create Commons license: Attribution-ShareAlike 4.0 International     */
/* (CC BY-SA 4.0)                                                       */
/* https://creativecommons.org/licenses/by-sa/4.0/                      */
/*                                                                      */
/* Flex lexer for parsing digital logic expressions into syntax tokens. */

%option noyywrap nodefault yylineno
%{
#include "main.h"
#include "b-parser.tab.h"
%}

%%
"+" |
"*" |
"^" |
"!" |
"|" |
"(" |
")"     { return yytext[0]; }
[a-zA-Z]{2}[0-9] {
    strcpy(yylval.s, yytext);
    return STRING;
}

\n      { return EOL; }
"//".*
[ \t]   { /* ignore whitespace */ }
.       { yyerror("Mystery character %c\n", *yytext); }
%%
