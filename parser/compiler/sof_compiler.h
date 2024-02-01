#ifndef SOF_COMPILER_H
#define SOF_COMPILER_H

#include <stdlib.h>

#include "../../pkg/error_codes.h"
#include "../structs.h"

#if defined(linux) || defined(__unix) || defined(__unix__) || defined(unix)
void compile(struct parser_array_token parser_tokens);

#elif defined(_WIN64)
void compile(struct parser_array_token parser_tokens);

#elif defined(_WIN32)
void compile(struct parser_array_token parser_tokens);
#endif

#endif // SOF_COMPILER_H