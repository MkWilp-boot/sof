#ifndef COMPILER_WIN64_H
#define COMPILER_WIN64_H

#include "../../common.h"
#include "../../../parser/structs.h"

#define MASM_CMD "ml64.exe ./asm/"ASM_FILE_NAME".asm /Fo ./objects"ASM_FILE_NAME".o  /Fe ./bin/"ASM_FILE_NAME".exe /nologo /W3 /link /entry:main"
#define WIN_CLEAR_COMPILATION_FILES "del "ASM_FILE_NAME".obj && del mllink$.lnk"

/**
 * @brief compiles a SOF file for a 64 bits Windows.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile_win64(struct parser_array_token parser_tokens);

#endif // COMPILER_WIN64_H