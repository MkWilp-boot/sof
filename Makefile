CC := gcc
C_version := -std=c11
C_flags := $(C_version) -Wall -pedantic

WIN_CUR_DIR := $(subst /,\,$(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

.PHONY: clean-linux
.PHONY: clean-win

win64: common_objs.o
	$(CC) -D W64 -c compiler/compiler.c -o objects/compiler.o $(C_flags)
	$(CC) -c compiler/windows/win64/win64.c -o objects/win64.o $(C_flags)
	$(CC) objects/sof.o objects/lexer.o objects/parser.o objects/compiler.o objects/win64.o -o sof $(C_flags)

linux: common_objs.o
	$(CC) -D L64 -c compiler/compiler.c -o objects/compiler.o $(C_flags)
	$(CC) -c compiler/linux/linux.c -o objects/linux.o $(C_flags)
	$(CC) objects/sof.o objects/lexer.o objects/parser.o objects/compiler.o objects/linux.o -o sof $(C_flags)

common_objs.o:
	$(CC) -c lexer/lexer.c -o objects/lexer.o $(C_flags)
	$(CC) -c parser/parser.c -o objects/parser.o $(C_flags)
	$(CC) -c main.c -o objects/sof.o $(C_flags) -g

clean-win:
	rmdir /s /q $(WIN_CUR_DIR)objects;
	rmdir /s /q $(WIN_CUR_DIR)bin;
	rmdir /s /q $(WIN_CUR_DIR)asm;
	mkdir $(WIN_CUR_DIR)objects;
	mkdir $(WIN_CUR_DIR)bin;
	mkdir $(WIN_CUR_DIR)asm;

clean-linux:
	rm -f sof
	find objects/* -delete
	find asm/* -delete
	find bin/* -delete