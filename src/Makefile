CC = gcc
CFLAGS = -std=gnu11

all: b-logic main.o and_or_not and_xor

b-logic: b-lexer.l b-parser.y main.h
	bison -d b-parser.y
	flex -o b-lexer.lex.c b-lexer.l

main.o:
	$(CC) $(CFLAGS) -c main.c

and_or_not: main.o b-parser.tab.c b-lexer.lex.c and_or_not.c
	$(CC) $(CFLAGS) and_or_not.c main.o b-parser.tab.c b-lexer.lex.c -o $@

and_xor: main.o b-parser.tab.c b-lexer.lex.c and_xor.c
	$(CC) $(CFLAGS) and_xor.c main.o b-parser.tab.c b-lexer.lex.c -o $@

clean:
	rm -f b-parser.tab.* b-lexer.lex.c *.pla *.o *.gch and_or_not and_xor
