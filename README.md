# b-logic
b-logic uses flex/bison to load a digital logic expression into an Abstract Syntax Tree. This tool will then minimize the expression into C2 AND-XOR form.

At the moment, this code parses arithmetic expressions into an AST and then evaluates the expression by recursively stepping through the tree.

### Running
```shell
$ cd src
$ make
$ ./calc
```

### Dependencies
* gcc
* flex
* bison
