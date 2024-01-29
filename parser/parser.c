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
static struct parser_token construct_number(const char* identifier, const char unary_operator) {
    struct parser_token token = {0};
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

    printf("%ld\n", token_data.i64_value);
    printf("%ld\n", token_data.u64_value);

    token.data = token_data;
    return token;
}

/**
 * @brief Constructs a struct parser_token operator from a char* identifier.
 *
 * @param identifier full identifier to be processed.
 * 
 * @attention This function is private to it's unit
 * 
 * @return parsed identifier.
 */
static struct parser_token construct_operator(const char* identifier) {
    struct parser_token token = {0};
    switch (identifier[0]) {
    case '-':
        token = (struct parser_token) {
            .type = PARSER_INT64,
            .operation = PARSER_SUB
        };
        break;
    case '+':
        token = (struct parser_token) {
            .type = PARSER_INT64,
            .operation = PARSER_SUM
        };
        break;
    default:
        exit(ERR_UNKNOW_OPERATOR);
        break;
    }
    return token;
}

struct parser_token *parser_tokenize(struct lexer_file_identifiers* file_identifiers) {
    struct parser_token *generated_tokens = (struct parser_token*)calloc(file_identifiers->size, sizeof(struct parser_token));

    for(size_t i = 0; i < file_identifiers->size; ++i) {
        const char* identifier = file_identifiers->identifiers[i];

        // unaries
        if(('-' == identifier[0] || '+' == identifier[0]) && '\0' != identifier[1]) {
            // identifier[0] is the sign of the identifier.
            struct parser_token token = construct_number(identifier, identifier[0]);
            generated_tokens[i] = token;
        }
        // single operators
        else if(('-' == identifier[0] || '+' == identifier[0]) && '\0' == identifier[1]) {
            struct parser_token token = construct_operator(identifier);
            generated_tokens[i] = token;
        }
        else {
            // always generates a positive numbers.
            struct parser_token token = construct_number(identifier, '+');
            generated_tokens[i] = token;
        }
    }

    // frees file_identfiers
    lexer_free_identifiers(file_identifiers);
    return NULL;
}