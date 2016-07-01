/**
 * b-logic/src/main.h
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
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

/* print nodes */
void traverse(struct ast *);

/* C2 AND-XOR form */
void and_xor(struct ast *);

/* and_or_not form */
void and_or_not(struct ast *);

/* convert AST to string */
char **ast_to_string(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

#endif
