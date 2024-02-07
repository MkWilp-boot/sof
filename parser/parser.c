#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "../lexer/structs.h"
#include "../lexer/lexer.h"

#include "parser.h"
#include "structs.h"

#define DEPENDENT_IDENTIFIERS_SIZE 32000

struct dependent_identifiers {
    // points to the symbol which depends on something
    uint32_t *depentent_identifers;

    // depentent_identifers size
    size_t size;
};

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
static inline parser_token_t construct_number(const char *identifier, const char unary_operator) {
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
static inline parser_token_t construct_operator(const char *identifier) {
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

static inline parser_token_t construct_symbol(const char *const identifier, struct dependent_identifiers *depentent_identifers) {
    if(0 == strcmp("print", identifier)) {
        enum parser_operation_type *before_dependencies = malloc(sizeof(enum parser_operation_type));
        before_dependencies[0] = PARSER_INT64;

        parser_token_t token = {
            .data = {0},
            .params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_PRINT,
            .pre_op_type_dependencies = before_dependencies,
            .post_op_type_dependencies = NULL,
            .pre_operations_dependencies = NULL,
            .post_operations_dependencies = NULL
        };
        return token;
    }
    else if(0 == strcmp("if", identifier)) {
        enum parser_operation *post_op_operations = malloc(sizeof(enum parser_operation));
        enum parser_operation *optional_operations = malloc(sizeof(enum parser_operation));
        enum parser_operation_type *pre_type_dependencies = malloc(sizeof(enum parser_operation_type));

        post_op_operations[0] = PARSER_END;
        optional_operations[0] = PARSER_ELSE;
        pre_type_dependencies[0] = PARSER_BOOL;

        parser_token_t token = {
            .data = {0},
            .params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_IF,
            .pre_op_type_dependencies = pre_type_dependencies,
            .post_op_type_dependencies = NULL,
            .optional_operations = optional_operations,
            .pre_operations_dependencies = NULL,
            .post_operations_dependencies = post_op_operations,
        };
        return token;
    }
    else if(0 == strcmp("end", identifier)) {
        enum parser_operation *pre_op_operations = malloc(sizeof(enum parser_operation));
        pre_op_operations[0] = PARSER_IF;

        parser_token_t token = {
            .data = {0},
            .params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_END,
            .pre_op_type_dependencies = NULL,
            .post_op_type_dependencies = NULL,
            .optional_operations = NULL,
            .pre_operations_dependencies = pre_op_operations,
            .post_operations_dependencies = NULL,
        };
        return token;
    }
    fprintf(stderr, "undefined identifier '%s'\n", identifier);
    exit(ERR_UNKNOW_IDENTIFIER);
}

struct parser_array_token parser_tokenize(struct lexer_file_identifiers *array_file_identifiers) {
    parser_token_t *generated_tokens = calloc(array_file_identifiers->size, sizeof(parser_token_t));

    uint32_t *depentent_identifers_ptr = calloc(DEPENDENT_IDENTIFIERS_SIZE, sizeof(uint32_t));
    struct dependent_identifiers depentent_identifers = {
        .depentent_identifers = depentent_identifers_ptr,
        .size = 0
    };

    char *discart_number_conversion;
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
        else if(strtol(identifier, &discart_number_conversion, 10) && *discart_number_conversion == '\0') {
            // always generates a positive numbers.
            parser_token_t token = construct_number(identifier, '+');
            generated_tokens[i] = token;

            // clean our trash
            discart_number_conversion = NULL;
        }
        else {
            printf("identifier: '%s'\n", identifier);
            parser_token_t token = construct_symbol(identifier, &depentent_identifers);
            generated_tokens[i] = token;
        }
    }
    free(depentent_identifers.depentent_identifers);

    // frees file_identfiers
    lexer_free_identifiers(array_file_identifiers);

    // still working
    exit(9999);

    struct parser_array_token parser_tokens = {
        .array = generated_tokens,
        .size = array_file_identifiers->size
    };
    return parser_tokens;
}