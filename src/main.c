/**
 * b-logic/src/main.c
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
 *
 * Contains functions for the creation and manipulation of digital logic
 * syntax trees.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"

struct ast *new_ast(int nodetype, struct ast *l, struct ast *r) {
    struct ast *a = (struct ast *)malloc(sizeof(struct ast));

    if(!a) {
        yyerror((char *)"error: malloc failed");
        exit(0);
    }

    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}


struct ast *new_node(char **d) {
    struct numval *a = (struct numval *)malloc(sizeof(struct numval));

    if(!a) {
        yyerror((char *)"error: malloc failed");
        exit(0);
    }

    a->nodetype = 'K';
    a->s = *d;
    printf("%s\n", *d);
    return (struct ast *)a;
}


void traverse(struct ast *a) {
    switch(a->nodetype) {

        /* two subtrees */
        case '+':
        case '*':
        case '^':
            printf("%c\n", a->nodetype);
            traverse(a->r);

        /* one subtree */
        case '|':
        case 'M':
            traverse(a->l);

        /* no subtree */
        case 'K':
            printf("%s\n", ((struct numval *)a)->s);
            break;
        default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}


void and_xor(struct ast *a) {
    ;
}


void and_or_not(struct ast *a) {
    ;
}


void ast_to_string(struct ast *a, char *d[]) {
    ;
}


void free_ast(struct ast *a) {
    switch(a->nodetype) {

        /* two subtrees */
        case '+':
        case '*':
        case '^':
            free_ast(a->r);

        /* one subtree */
        case '|':
        case 'M':
            free_ast(a->l);

        /* no subtree */
        case 'K':
            free(a);
            break;
        default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}


void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}


int main() {
    printf("> ");
    return yyparse();
}
