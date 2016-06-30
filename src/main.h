/**
 * b-logic/src/main.h - John Burnett (c) 2016
 */

#ifndef main_h
#define main_h

/* interface to the lexer */
extern int yylineno; /* from lexer */
void yyerror(char *s, ...);

/* nodes in the abstract syntax tree */
struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct numval {
    int nodetype; /* type K for constant */
    int number;
};

/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(int d);

/* evaluate an AST */
int eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

#endif
