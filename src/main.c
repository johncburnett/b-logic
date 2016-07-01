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
 * Contains functions for the creation and manipulation digital logic
 * syntax trees.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"

struct ast *
newast(int nodetype, struct ast *l, struct ast *r) {
    struct ast *a = malloc(sizeof(struct ast));

    if(!a) {
        yyerror("out of space");
        exit(0);
    }

    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
}


struct ast *
newnum(int d) {
    struct numval *a = malloc(sizeof(struct numval));

    if(!a) {
        yyerror("out of space");
        exit(0);
    }

    a->nodetype = 'K';
    a->number = d;
    return (struct ast *)a;
}


int
eval(struct ast *a) {
    int v; /* calculated value of this subtree */

    switch(a->nodetype) {
        case 'K': v = ((struct numval *)a)->number; break;
        case '+': v = eval(a->l) | eval(a->r); break;
        case '*': v = eval(a->l) & eval(a->r); break;
        case '^': v = eval(a->l) ^ eval(a->r); break;
        case '|': v = eval(a->l); if(v < 0) v = -v; break;
        case 'M': v = -eval(a->l); break;
        default: printf("internal error: bad node %c\n", a->nodetype);
    }

    return v;
}


void
traverse(struct ast *a) {
    switch(a->nodetype) {

        /* two subtrees */
        case '+':
        case '*':
        case '^':
            traverse(a->r);

        /* one subtree */
        case '|':
        case 'M':
            traverse(a->l);

        /* no subtree */
        case 'K':
            printf("%d\n", ((struct numval *)a)->number);
            break;
        default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}


void
and_xor(struct ast *a) {
    ;
}


void
and_or_not(struct ast *a) {
    ;
}


char **
ast_to_string(struct ast *a) {
    ;
}


void
treefree(struct ast *a) {
    switch(a->nodetype) {

        /* two subtrees */
        case '+':
        case '*':
        case '^':
            treefree(a->r);

        /* one subtree */
        case '|':
        case 'M':
            treefree(a->l);

        /* no subtree */
        case 'K':
            free(a);
            break;
        default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}


void
yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}


int
main() {
    printf("> ");
    return yyparse();
}
