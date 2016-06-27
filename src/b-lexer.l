/* b-logic/src/b-lexer.l - John Burnett (c) 2016 */

/* recognize tokens for the calculator */
%option noyywrap nodefault yylineno
%{
# include "main.h"
# include "b-parser.tab.h"
%}

/* float exponent */
EXP     ([Ee][-+]?[0-9]+)

%%
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")"     { return yytext[0]; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.d = atof(yytext); return NUMBER; }

\n      { return EOL; }
"//".*
[ \t]   { /* ignore whitespace */ }
.       { yyerror("Mystery character %c\n", *yytext); }
%%
