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

// interface with lexer
extern int yylineno;
void yyerror(char *s, ...);

// abstract syntax tree data structure
struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct numval {
    int nodetype; // type K for constant
    char *s;
    int val;
};

// list of tokens
struct numval *tokens[100];
int num_tokens;

// build an AST
struct ast *new_ast(int nodetype, struct ast *l, struct ast *r);
struct ast *new_node(char** d);

// print nodes
void traverse(struct ast *);

// delete and free an AST
void free_ast(struct ast *);

// add tokens to list
void add_token(struct numval *);

// convert AST to PLA and write to .pla file
void generate_pla(struct ast *);

// evaluate ast
int eval(struct ast *);

// C2 AND-XOR form
void and_xor(struct ast *);

// and_or_not form
void and_or_not(struct ast *);

// convert AST to string
void ast_to_string(struct ast *, char **);

#endif
