#ifndef SOF_PARSER_H
#define SOF_PARSER_H

#include "../lexer/structs.h"
#include "structs.h"

#define IS_OPERATOR(op) \
    '+' == op || \
    '-' == op || \
    '=' == op

#define IS_UNARY_OPERATION(fst, snd) \
    ('-' == fst || '+' == fst) && '\0' != snd

#define IS_INT_LIKE(t) \
    t == PARSER_INT64 || \
    t == PARSER_INT32 || \
    t == PARSER_UINT64 || \
    t == PARSER_UINT32

#define PRODUCE_INT_LIKE(t) \
    t == PARSER_EQ || \
    t == PARSER_SUM || \
    t == PARSER_SUB

/**
 * @brief creates tokens based on a input file.
 *
 * @attention This function frees the parameter file_identifiers
 * 
 * @param file_identifiers File processed by the lexer.
 * @return array of parser_token.
 */
struct parser_array_token parser_tokenize(struct lexer_file_identifiers *array_file_identifiers);

#endif // SOF_PARSER_H