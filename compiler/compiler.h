#ifndef COMPILER_H
#define COMPILER_H

#include <stdlib.h>

#include "../pkg/error_codes.h"
#include "../parser/structs.h"

/**
 * @brief compiles a SOF file.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile(struct parser_array_token parser_tokens);
#endif // COMPILER_H