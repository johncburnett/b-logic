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

#include <stdio.h>

// interface with lexer
extern int yylineno;
void yyerror(char *s, ...);
int yylex();
int yyparse();
extern FILE * yyin;

// abstract syntax tree data structure
struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

// holds varable name and value of a leaf
struct numval {
    int nodetype; // type K for constant
    char *s;
    int val;
};

// holds strings and values for each term in a minterm
struct minterm {
    char *terms[100];
    int values[100];
    int n;
};

// used to generate c2 terms
struct indices {
    int v[100];
    int n;
};

// root of AST
struct ast *root;

// list of individual variable names
char *var_names[100];
int num_vars;

// array holding pointers to each leaf in AST
struct numval *tokens[100];
int num_tokens;

// list of minterms
struct minterm *expression[100];
int num_minterms;

// array of possible c2 terms
struct indices *c2_terms;
int c2_len;

// AST functions
struct ast *new_ast(int nodetype, struct ast *l, struct ast *r); // build AST
struct ast *new_node(char** d); // add a leaf
void traverse(struct ast *); // print nodes
int eval(struct ast *); // evaluate ast
void free_ast(struct ast *); // delete and free an AST

// token list functions
void add_token(struct numval *); // add token to array
void empty_tokens(void); // free array of tokens

// convert AST to PLA and write to .pla file
void generate_pla(struct ast *);

// and_or_not form
void and_or_not(struct ast *);

// C2 AND-XOR form
void and_xor(struct ast *);
int choose(int, int);
void combinations(int, int, int, struct indices, int);

// changes notation of espresso output
void reformat_output(void);

// generate minterms from PLA
void generate_minterms(struct ast *);
void minterms_to_ascii(void);

// print contents of a file
void print_file(char **fname);

#endif
