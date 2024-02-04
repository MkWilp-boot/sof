#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "../lexer/structs.h"
#include "../lexer/lexer.h"

#include "parser.h"
#include "structs.h"

/**
 * @brief Constructs a struct parser_token number from a char* identifier.
 *
 * @param identifier full identifier to be processed.
 * @param unary_operator sign of the identifier.
 * 
 * @attention This function is private to it's unit
 * 
 * @return parsed identifier.
 */
static parser_token_t construct_number(const char *identifier, const char unary_operator) {
    parser_token_t token = {0};
    union parser_data token_data = {0};

    token.params = NULL;
    token.operation = PARSER_PUSH;
    token.type = unary_operator == '-' ? PARSER_INT64 : PARSER_UINT64;

    switch(token.type) {
    case PARSER_INT64: 
        token_data = (union parser_data) { .i64_value = atoi(identifier) };
        break;
    case PARSER_UINT64:
        token_data = (union parser_data) { .u64_value = atoi(identifier) };
        break;
    default:
        exit(ERR_UNKNOW);
    }

    token.data = token_data;
    return token;
}

/**
 * @brief Constructs a struct parser_token operator from a char *identifier.
 *
 * @param identifier full identifier to be processed.
 * 
 * @attention This function is private to it's unit
 * 
 * @return parsed identifier.
 */
static parser_token_t construct_operator(const char *identifier) {
    parser_token_t token = {0};
    switch (identifier[0]) {
    case '-':
        token = (parser_token_t) {
            .type = PARSER_INT64,
            .operation = PARSER_SUB
        };
        break;
    case '+':
        token = (parser_token_t) {
            .type = PARSER_INT64,
            .operation = PARSER_SUM
        };
        break;
    case '=':
        token = (parser_token_t) {
            .type = PARSER_BOOL,
            .operation = PARSER_EQ
        };
        break;
    default:
        fprintf(stderr, "Unknow identifier '%c'\n", identifier[0]);
        exit(ERR_UNKNOW_OPERATOR);
        break;
    }
    return token;
}

struct parser_array_token parser_tokenize(struct lexer_file_identifiers *array_file_identifiers) {
    parser_token_t *generated_tokens = calloc(array_file_identifiers->size, sizeof(parser_token_t));

    for(size_t i = 0; i < array_file_identifiers->size; ++i) {
        const char *const identifier = array_file_identifiers->identifiers[i];

        if(IS_UNARY_OPERATION(identifier[0], identifier[1])) {
            // identifier[0] is the sign of the identifier.
            parser_token_t token = construct_number(identifier, identifier[0]);
            generated_tokens[i] = token;
        }
        // single operators
        else if(IS_OPERATOR(identifier[0])) {
            parser_token_t token = construct_operator(identifier);
            generated_tokens[i] = token;
        }
        else {
            // always generates a positive numbers.
            parser_token_t token = construct_number(identifier, '+');
            generated_tokens[i] = token;
        }
    }

    // frees file_identfiers
    lexer_free_identifiers(array_file_identifiers);

    struct parser_array_token parser_tokens = {
        .array = generated_tokens,
        .size = array_file_identifiers->size
    };
    return parser_tokens;
}