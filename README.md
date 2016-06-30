# b-logic
b-logic uses flex/bison to load a digital logic expression into an Abstract Syntax Tree. This tool will then minimize the expression into C2 AND-XOR form.

At the moment, this code simply parses boolean expressions into an AST and then evaluates the expression recursively.

### Running
```shell
$ cd src
$ make
$ ./b-logic
> ((1^0)+(1*0)) + (0^0)   # type some logic
```

### Dependencies
* gcc
* flex
* bison
