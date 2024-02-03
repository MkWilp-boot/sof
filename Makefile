CC := gcc
C_version := -std=c11
C_flags := $(C_version) -Wall -pedantic

.PHONY: clean-linux
.PHONY: clean-windows
.PHONY: sof

sof: objects.o
	$(CC) objects/sof.o objects/lexer.o objects/parser.o objects/sof_compiler.o objects/linux.o objects/win64.o -o sof $(C_flags)

objects.o:
	$(CC) -c lexer/lexer.c -o objects/lexer.o $(C_flags)
	$(CC) -c parser/parser.c -o objects/parser.o $(C_flags)
	$(CC) -c parser/compiler/sof_compiler.c -o objects/sof_compiler.o $(C_flags)
	$(CC) -c parser/compiler/linux/linux.c -o objects/linux.o $(C_flags)
	$(CC) -c parser/compiler/windows/win64/win64.c -o objects/win64.o $(C_flags)
	$(CC) -c main.c -o objects/sof.o $(C_flags) -g

clean-windows:

clean-linux:
	rm -f sof
	find objects/* -delete
	find asm/* -delete
	find bin/* -delete