#ifndef SOF_PARSER_H
#define SOF_PARSER_H

#include "../lexer/structs.h"
#include "../pkg/collections/vector/vector.h"

#include "structs.h"

#define IS_OPERATOR(op) \
    '+' == op || \
    '-' == op || \
    '=' == op

#define IS_UNARY_OPERATION(fst, snd) \
    ('-' == fst || '+' == fst) && '\0' != snd

#define IS_INT_LIKE(t) \
    ( t == PARSER_INT64 || \
      t == PARSER_INT32 || \
      t == PARSER_UINT64 || \
      t == PARSER_UINT32 )

#define PRODUCE_INT_LIKE(t) \
    ( t == PARSER_EQ || \
      t == PARSER_SUM || \
      t == PARSER_SUB )

#define PRODUCE_BOOL(t) \
    t == PARSER_EQ

#define CAN_LINK_TO_END(t) \
    (t->token->operation == PARSER_IF)

/**
 * @brief creates tokens based on a input file.
 *
 * @attention This function frees the parameter array_file_identifiers
 * 
 * @param array_file_identifiers File processed by the lexer.
 * @return array of parser_token.
 */
vector_t parser_tokenize(vector_t *array_file_identifiers);

#endif // SOF_PARSER_H