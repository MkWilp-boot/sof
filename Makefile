CFLAGS := -Wall -std=c11 -pedantic

main: objects.o
	cc objects/main.o objects/lexer.o objects/parser.o -o main $(CFLAGS)

objects.o:
	cc -c lexer/lexer.c -o objects/lexer.o $(CFLAGS)
	cc -c parser/parser.c -o objects/parser.o $(CFLAGS)
	cc -c main.c -o objects/main.o $(CFLAGS) -g

clean:
	rm -f main
	find objects/* -delete