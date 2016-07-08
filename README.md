# b-logic
b-logic uses flex/bison to load a digital logic expression into an Abstract Syntax Tree. This tool will then minimize the expression into Sum of Products AND-OR-NOT form as well as C2 AND-XOR form.

As of now, the the program runs as a prompt and accepts digital logic expressions using the operators "+, \*, ^, !". The prompt returns espresso's SoP minimization of the input.

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
* espresso logic minimizer (accessible from $PATH)
