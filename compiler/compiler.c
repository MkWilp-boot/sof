#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "compiler.h"

#include "../parser/structs.h"

#include "../pkg/collections/vector/vector.h"

#if defined(W64)

#include "windows/win64/win64.h"
void compile(vector_t parser_tokens) {
    compile_win64(parser_tokens);
}

#elif defined(L64)

#include "linux/linux.h"
void compile(vector_t parser_tokens) {
    compile_linux(parser_tokens);
}
#else
void compile(vector_t parser_tokens) {
    assert(0 && "Undefined compiler option");
}
#endif