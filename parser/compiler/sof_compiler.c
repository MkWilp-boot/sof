#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "sof_compiler.h"

#include "../structs.h"

#if defined(W64)
void compile(struct parser_array_token parser_tokens) {
    compile_win64(parser_tokens);
}

#elif defined(L64)
void compile(struct parser_array_token parser_tokens) {
    compile_linux(parser_tokens);
}
#else
void compile(struct parser_array_token parser_tokens) {
    assert(0 && "Undefined compiler option");
}
#endif