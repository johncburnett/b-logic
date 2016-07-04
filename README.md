# b-logic
b-logic uses flex/bison to load a digital logic expression into an Abstract Syntax Tree. This tool will then minimize the expression into C2 AND-XOR form.

At the moment, this code simply parses boolean expressions into an AST and prints a traversal of the tree.

### Running
```shell
$ cd src
$ make
$ ./b-logic
> ((FF1^AB0)+(AB1*FF2))+(FF0^AA0)   # type some logic
```

### Dependencies
* gcc
* flex
* bison
