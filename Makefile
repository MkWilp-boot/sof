CFLAGS := -Wall -std=c11 -pedantic

main: objects.o
	cc objects/main.o objects/lexer.o -o main $(CFLAGS)

objects.o:
	cc -c lexer/lexer.c -o objects/lexer.o $(CFLAGS)
	cc -c main.c -o objects/main.o $(CFLAGS)

clean:
	rm -f main
	find objects/* -delete