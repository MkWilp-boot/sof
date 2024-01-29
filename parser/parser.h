#ifndef SOF_PARSER_H
#define SOF_PARSER_H

#include "../lexer/structs.h"
#include "structs.h"

/**
 * @brief creates tokens based on a input file.
 *
 * @attention This function frees the parameter file_identifiers
 * 
 * @param file_identifiers File processed by the lexer.
 * @return array of parser_token.
 */
struct parser_token* parser_tokenize(struct lexer_file_identifiers* file_identifiers);

#endif // SOF_PARSER_H