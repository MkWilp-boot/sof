#ifndef SOF_COMPILER_LINUX_H
#define SOF_COMPILER_LINUX_H

#include "../common.h"

#define NASM_CMD "nasm -f elf64 ./asm/"ASM_FILE_NAME".asm -o ./objects/"ASM_FILE_NAME".o"
#define LINKER_CMD "ld ./objects/"ASM_FILE_NAME".o -o ./bin/"ASM_FILE_NAME".run"

/**
 * @brief compiles a SOF file for linux systems.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile_linux(struct parser_array_token parser_tokens);

#endif // SOF_COMPILER_LINUX_H