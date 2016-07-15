/*
 * b-logic/src/main.c
 *
 * Copyright 2016 - John Bass, DMS Design
 * Copyright 2016 - John Burnett, Developer
 *
 * Create Commons license: Attribution-ShareAlike 4.0 International
 * (CC BY-SA 4.0)
 * https://creativecommons.org/licenses/by-sa/4.0/
 *
 * Functions for the creation of ASTs and the conversion of logic expressions
 * to Sum-of-Products AND-OR-NOT form and C2 Cannonical AND-XOR form.
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


int eval(struct ast *a) {
    int v;

    switch(a->nodetype) {
        case 'K': v = ((struct numval *)a)->val; break;
        case '1': v = 1; break;
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
        case '1':
            free(a);
            break;
        default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}


void add_token(struct numval *a) {
    if(num_vars == 0) {
        var_names[num_vars] = a->s;
        num_vars++;
    } else {
        for(int i = 0; i < num_vars; i++) {
            if(strcmp(a->s, var_names[i]) == 0) {
                break;
            }
            else if(i == num_vars-1) {
                var_names[num_vars] = a->s;
                num_vars++;
                break;
            }
        }
    }
    tokens[num_tokens] = a;
    num_tokens++;
}


void set_node(int var_index, int bit) {
    for(int i = 0; i < num_tokens; i++) {
        if(strcmp(var_names[var_index], tokens[i]->s) == 0) {
            tokens[i]->val = bit;
        }
    }
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
 * for every variable) and evaluate the tree for every combination of truth values
 */
void generate_pla(struct ast *a) {
    int counter = 0;
    int max = 1 << num_vars;
    int result = 0;
    FILE *pla = fopen("in.pla", "w+");

    // format pla file header
    fprintf(pla, ".i %d\n", num_vars);
    fprintf(pla, ".o 1\n");
    fprintf(pla, ".ilb ");
    for(int i = num_vars-1; i >= 0; i--) {
        fprintf(pla, "%s ", var_names[i]);
    }
    fprintf(pla, "\n.ob %s\n", "F");

    // generate and write pla
    for(int i = 0; i < max; i++) {
        for(int j = num_vars-1; j >= 0; j--) {
            int is_set = counter & (1 << j); // check jth bit
            int bit = 0;
            if(is_set) bit = 1;
            set_node(j, bit);
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
 * to minimize to SoP, we simply need to run our PLA file through the espresso
 * executable by forking a child process
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
            fprintf(stderr, "internal error: fork failed\n");
            exit(1);
        }
        wait(&status); // wait for child
        dup2(saved_stdout, 1); // restore stdout
        close(saved_stdout);
    }
}


/*
 * and_xor
 *
 * recursive generation of c2 variables and generation of c2 form
 */
void and_xor(struct ast *a) {
    // calculate number of possible c2 terms
    int num_c2_terms = 0;
    for(int i = 0; i < num_vars; i++) {
        num_c2_terms += choose(num_vars, i+1);
    }

    // initialize table
    int c2_table[num_minterms][num_c2_terms];
    for(int i = 0; i < num_minterms; i++) {
        for(int j = 0; j < num_c2_terms; j++) {
            c2_table[i][j] = 0;
        }
    }

    // generate all possible c2 terms (using the index of the variable)
    c2_terms = (struct indices *)malloc(sizeof(struct indices)*num_c2_terms);
    c2_len = 0;
    for(int i = 1; i < num_vars+1; i++) {
        struct indices r;
        r.n = i;
        combinations(num_vars, i, 0, r, i);
    }

    // search terms and set table
    for(int i = 0; i < num_minterms; i++) {
        for(int j = 0; j < expression[i]->n; j++) {
            if(expression[i]->values[j] == 1) {
                for(int k = 0; k < num_c2_terms; k++) {
                    for(int l = 0; l < c2_terms[k].n; l++) {
                        if(j == c2_terms[k].v[l]) {
                            c2_table[i][k] = 1;
                        }
                    }
                }
            }
        }
    }

    int c2_final[num_c2_terms];
    int c2_last_index;

    // count columns, if odd then include term
    for(int j = 0; j < num_c2_terms; j++) {
        int total = 0;
        for(int i = 0; i < num_minterms; i++) {
            if(c2_table[i][j] == 1) {
                total++;
            }
        }
        c2_final[j] = total % 2;
        if(c2_final[j]) {
            c2_last_index = j;
        }
    }

    int is_empty = 1;
    for(int i = 0; i < num_c2_terms; i++) {
        if(c2_final[i] == 1) {
            is_empty = 0;
        }
    }
    if(is_empty) {
        printf("error: not expressable in C2 form");
    }

    FILE *fp = fopen("out.pla", "w+");
    for(int i = 0; i < c2_last_index+1; i++) {
        if(c2_final[i] == 1) {
            fprintf(fp, "(");
            for(int j = 0; j < c2_terms[i].n; j++) {
                int index = c2_terms[i].v[j];
                fprintf(fp, "%s", expression[0]->terms[index]);
                if(j != c2_terms[i].n-1) {
                    fprintf(fp, "*");
                }
            }
            fprintf(fp, ")");
            if(i != c2_last_index) {
                fprintf(fp, "^");
            }
        }
    }
    fprintf(fp, "\n");
    fclose(fp);

    free(c2_terms);
}


/*
 * choose
 *
 * helper function for calculating (n choose k)
 */
int choose(int n, int k) {
    if(k==0)    return 1;
    else        return (n * choose(n-1, k-1)) / k;
}


/*
 * combinations
 *
 * helper function for determining all n-length combinations of a set
 */
void combinations(int s_len, int len, int start, struct indices r, int r_len) {
    if(len == 0) {
        c2_terms[c2_len] = r;
        c2_len++;
        return;
    }
    for(int i = start; i <= s_len - len; i++) {
        r.v[r_len - len] = i;
        combinations(s_len, len-1, i+1, r, r_len);
    }
}


/*
 * reformat_pla
 *
 * erases the LHS and switches operator notation of espresso's output
 */
void reformat_output(void) {
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
        else if(c == '\n' || c == ' ') {
            continue;
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


/*
 * generate_minterms
 *
 * determines minterms from an AST
 */
void generate_minterms(struct ast *a) {
    for(int i = 0; i < (1 << num_vars); i++) {
        struct minterm *mt = (struct minterm *)malloc(sizeof(struct minterm));
        mt->n = num_vars;
        for(int j = 0; j < num_vars; j++) {
            int is_set = i & (1 << j); // check jth bit
            int bit = 0;
            if(is_set) bit = 1;
            set_node(j, bit);
            mt->terms[j] = var_names[j];
            mt->values[j] = bit;
        }
        if(eval(a) == 1) {
            expression[num_minterms] = mt;
            num_minterms++;
        } else {
            free(mt);
        }
    }
}


/*
 * minterms_to_ascii
 *
 * writes minterms in XOR Sum-of-Products form to a file
 */
void minterms_to_ascii(void) {
    FILE *fp = fopen("in.pla", "w+");
    for(int i = 0; i < num_minterms; i++) {
        fprintf(fp, "(");
        for(int j = 0; j < expression[i]->n; j++) {
            if(expression[i]->values[j] == 0) {
                fprintf(fp, "!");
            }
            fprintf(fp, "%s", expression[i]->terms[j]);
            if(j != expression[i]->n - 1) {
                fprintf(fp, "*");
            }
        }
        fprintf(fp, ")");
        if(i != num_minterms-1) {
            fprintf(fp, "^");
        }
    }
    fprintf(fp, "\n");
    fclose(fp);
}


/*
 * print_file
 *
 * print the contents of a file
 */
void print_file(char **fname) {
    FILE *fp = fopen(*fname, "r");
    int c;
    while ((c = getc(fp)) != EOF) {
        putchar(c);
    }
    fclose(fp);
}


void yyerror(char *s, ...) {
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
