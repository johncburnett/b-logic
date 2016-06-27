all: calc

calc: b-lexer.l b-parser.y main.h
	bison -d b-parser.y
	flex -o b-lexer.lex.c b-lexer.l
	cc -o $@ b-parser.tab.c b-lexer.lex.c main.c

clean:
	rm b-parser.tab.* b-lexer.lex.c calc
