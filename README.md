# b-logic
b-logic uses flex/bison to parse a digital logic expression into an abstract syntax tree. This tool then minimizes the expression into Sum of Products AND-OR-NOT form as well as C2 AND-XOR form.

These programs - the executables `and_or_not` and `and_xor` - accept digital logic expressions which use the operators "+, \*, ^, !". These programs write the converted input to `in.pla` and the minimizations to `out.pla`. Input can be given through either `stdin` or a file passed as a command line argument. Variable names are of the form `[a-zA-Z][a-zA-Z0-9]*`.

### Running
```shell
$ cd src
$ make
$ ./and_or_not
> ((FF1^AB0)+(AB1*FF2))     # type some logic
$ ./and_or_not input.txt    # or provide a file
```

### Dependencies
* gcc
* flex
* bison
* espresso logic minimizer (accessible from $PATH)
