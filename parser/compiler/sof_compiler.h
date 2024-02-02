#ifndef SOF_COMPILER_H
#define SOF_COMPILER_H

#include <stdlib.h>

#include "../../pkg/error_codes.h"
#include "../structs.h"

#if defined(linux) || defined(__unix) || defined(__unix__) || defined(unix)
/**
 * @brief compiles a SOF file for linux systems.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile(struct parser_array_token parser_tokens);

#elif defined(_WIN64)
/**
 * @brief compiles a SOF file for windows 64 bits systems.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile(struct parser_array_token parser_tokens);

#elif defined(_WIN32)
/**
 * @brief compiles a SOF file for windows 32 bits systems.
 *
 * @param parser_tokens tokens to be processed.
 */
void compile(struct parser_array_token parser_tokens);
#endif

#endif // SOF_COMPILER_H