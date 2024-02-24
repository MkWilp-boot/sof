#ifndef COMPILER_H
#define COMPILER_H

#include <stdlib.h>

#include "../pkg/error_codes.h"
#include "../pkg/collections/vector/vector.h"
#include "../parser/structs.h"

/**
 * @brief compiles a SOF file.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile(vector_t parser_tokens);
#endif // COMPILER_H