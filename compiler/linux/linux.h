#ifndef COMPILER_LINUX_H
#define COMPILER_LINUX_H

#include "../common.h"
#include "../../pkg/collections/vector/vector.h"

#define NASM_CMD "nasm -f elf64 ./asm/"ASM_FILE_NAME".asm -o ./objects/"ASM_FILE_NAME".o"
#define LINKER_CMD "ld ./objects/"ASM_FILE_NAME".o -o ./bin/"ASM_FILE_NAME".run"

/**
 * @brief compiles a SOF file for linux systems.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile_linux(vector_t parser_tokens);

#endif // COMPILER_LINUX_H