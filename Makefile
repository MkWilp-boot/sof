CC := gcc
C_VERSION := -std=c11
C_FLAGS := $(C_VERSION) -Wall -pedantic
RELEASE_FLAGS := -O2

.PHONY: clean

sof: objects.o
	$(CC) objects/sof.o objects/lexer.o objects/parser.o objects/sof_compiler.o -o sof $(C_FLAGS)

objects.o:
	$(CC) -c lexer/lexer.c -o objects/lexer.o $(C_FLAGS)
	$(CC) -c parser/parser.c -o objects/parser.o $(C_FLAGS)
	$(CC) -c parser/compiler/sof_compiler.c -o objects/sof_compiler.o $(C_FLAGS)
	$(CC) -c main.c -o objects/sof.o $(C_FLAGS) -g

clean:
	rm -f sof
	find objects/* -delete