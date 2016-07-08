CC = gcc
CFLAGS = -Wall

all: b-logic

b-logic: b-lexer.l b-parser.y main.h
	bison -d b-parser.y
	flex -o b-lexer.lex.c b-lexer.l
	$(CC) $(CFLAGS) -o $@ b-parser.tab.c b-lexer.lex.c main.c

clean:
	rm -f b-parser.tab.* b-lexer.lex.c b-logic *.pla
