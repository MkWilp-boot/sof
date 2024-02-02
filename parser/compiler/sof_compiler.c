#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "sof_compiler.h"

#include "../structs.h"

#if defined(linux) || defined(__unix) || defined(__unix__) || defined(unix)

#include "linux/linux.h"
void compile(struct parser_array_token parser_tokens) {
    compile_linux(parser_tokens);
}

#elif defined(_WIN64)
void compile(struct parser_array_token parser_tokens) {
    assert(0 && "win64 not implemented");
}

#elif defined(_WIN32)
void compile(struct parser_array_token parser_tokens) {
    assert(0 && "win32 not implemented");
}
#endif