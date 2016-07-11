/*
 * b-logic/src/main.c
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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
    a->val = 0;
    add_token(a);

    return (struct ast *)a;
}


void traverse(struct ast *a) {
    switch(a->nodetype) {

        // two subtrees
        case '+':
        case '*':
        case '^':
            printf("%c\n", a->nodetype);
            traverse(a->r);

        // one subtree
        case '!':
            traverse(a->l);

        // no subtree
        case 'K':
            printf("%s\n", ((struct numval *)a)->s);
            break;
        default: printf("internal error: found bad node %c\n", a->nodetype);
    }
}


int eval(struct ast *a) {
    int v;

    switch(a->nodetype) {
        case 'K': v = ((struct numval *)a)->val; break;
        case '+': v = eval(a->l) | eval(a->r); break;
        case '*': v = eval(a->l) & eval(a->r); break;
        case '^': v = eval(a->l) ^ eval(a->r); break;
        case '!': v = !eval(a->l); break;
        default: printf("internal error: bad node %c\n", a->nodetype); }
    return v;
}


void free_ast(struct ast *a) {
    switch(a->nodetype) {

        // two subtrees
        case '+':
        case '*':
        case '^':
            free_ast(a->r);

        // one subtree
        case '!':
            free_ast(a->l);

        // no subtree
        case 'K':
            free(a);
            break;
        default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}


void add_token(struct numval *a) {
    for(int i = 0; i < num_tokens; i++) {
        if(strcmp(a->s, tokens[i]->s) != 0) {
            break;
        }
    }
    tokens[num_tokens] = a;
    num_tokens++;
}


void empty_tokens(void) {
    for(int i = 0; i < num_tokens; i++) {
        tokens[i] = NULL;
    }
    num_tokens = 0;
}


/*
 * generate_pla
 *
 * to convert our AST into a PLA, we iterate through an n-bit number (one bit
 * for every token) and evaluate the tree for every combination of truth values
 */
void generate_pla(struct ast *a) {
    int counter = 0;
    int max = 1 << num_tokens;
    int result = 0;
    FILE *pla = fopen("in.pla", "w+");

    // format pla file header
    fprintf(pla, ".i %d\n", num_tokens);
    fprintf(pla, ".o 1\n");
    fprintf(pla, ".ilb ");
    for(int i = num_tokens-1; i >= 0; i--) {
        fprintf(pla, "%s ", tokens[i]->s);
    }
    fprintf(pla, "\n.ob %s\n", "F");

    // generate and write pla
    for(int i = 0; i < max; i++) {
        for(int j = num_tokens-1; j >= 0; j--) {
            int is_set = counter & (1 << j); // check jth bit
            int bit = 0;
            if(is_set) bit = 1;
            tokens[j]->val = bit;
            fprintf(pla, "%d", bit);
        }
        result = eval(a);
        fprintf(pla, " %i\n", result);
        counter++;
    }
    fprintf(pla, ".e\n");
    fclose(pla);
}


/*
 * and_or_not
 *
 * to minimize to SoP, we simply need to run our PLA file through espresso
 */
void and_or_not(struct ast *a) {
    char *args[5];
    args[0] = (char *)"espresso";
    args[1] = (char *)"-o";
    args[2] = (char *)"eqntott";
    args[3] = (char *)"in.pla";
    args[4] = NULL;

    pid_t pid;
    int status;

    FILE *f_out = freopen("out.pla", "w+", stdout); // open output file
    int fd = fileno(f_out); // get file descriptor
    int saved_stdout = dup(1); // save stdout
    dup2(fd, 1); // replace stdout

    if((pid=fork()) == 0) { // fork to run espresso executable
        execvp(*args, args);
        fprintf(stderr, "internal error: espresso failed to run\n");
        exit(1);
    }
    else { // parent
        if(pid == (pid_t)(-1)) {
            fprintf(stderr, "internal error: fork failed\n"); exit(1);
        }
        wait(&status); // wait for child
        dup2(saved_stdout, 1); // restore stdout
        close(saved_stdout);
    }
}


/*
 * and_xor
 *
 * TODO
 */
void and_xor(struct ast *a) {
    ;
}


/*
 * reformat_pla
 *
 * erases the LHS and switches operator notation of espresso's output
 */
void reformat_pla(void) {
    FILE *fp = fopen("out.pla", "r"); // open espresso output

    char c;
    char output[1000];
    int i = 0;

    for(int j = 0; j < 4; j++) {
        fgetc(fp); // skip over LHS
    }
    while( (c=fgetc(fp)) != ';' ) {
        if(c == '&') {
            output[i] = '*';
        }
        else if(c == '|') {
            output[i] = '+';
        }
        else {
            output[i] = c;
        }
        i++;
    }
    output[i] = '\0';

    // overwrite file
    freopen(NULL, "w+", fp);
    fprintf(fp, "%s\n", output);
    fclose(fp);
}


void print_file(char **fname) {
    FILE *fp = fopen(*fname, "r");
    int c;
    while ((c = getc(fp)) != EOF) {
        putchar(c);
    }
    fclose(fp);
}


void ast_to_string(struct ast *a, char **d) {
    ;
}


void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
