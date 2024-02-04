#ifndef SOF_COMPILER_H
#define SOF_COMPILER_H

#include <stdlib.h>

#include "../../pkg/error_codes.h"
#include "../structs.h"

/**
 * @brief compiles a SOF file.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile(struct parser_array_token parser_tokens);
#endif // SOF_COMPILER_H