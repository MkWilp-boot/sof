#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../lexer/structs.h"
#include "../lexer/lexer.h"

#include "parser.h"
#include "structs.h"

struct dependent_identifiers {
    // points to the symbol which depends on something
    uint32_t *depentent_identifers;

    // depentent_identifers size
    size_t size;
};

/**
 * @brief Constructs a struct parser_token_t number from a char* identifier.
 *
 * @param identifier full identifier to be processed.
 * @param unary_operator sign of the identifier.
 * 
 * @attention This function is private to it's unit
 * 
 * @return parsed identifier.
 */
static parser_token_t construct_number(const char *identifier, const char unary_operator) {
    parser_token_t token;
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
 * @brief Constructs a struct parser_token_t operator from a char *identifier.
 *
 * @param identifier full identifier to be processed.
 * 
 * @attention This function is private to it's unit
 * 
 * @return parsed identifier.
 */
static parser_token_t construct_operator(const char *identifier) {
    parser_token_t token;
    switch (identifier[0]) {
    case '-':
        token = (parser_token_t) {
            .type = PARSER_INT_LIKE,
            .operation = PARSER_SUB
        };
        break;
    case '+':
        token = (parser_token_t) {
            .type = PARSER_INT_LIKE,
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

/**
 * @brief Constructs a struct parser_token_t operator from a char *identifier.
 *
 * @param ip Instruction pointer
 * @param identifier Full identifier to be processed.
 * @param  array_dependent_identifiers Array of tokens that are dependent on something else
 * 
 * @attention This function is private to it's unit
 * 
 * @return parsed identifier.
 */
static parser_token_t construct_symbol(const size_t ip, const char *const identifier, struct dependent_identifiers *array_dependent_identifiers) {
    if(0 == strcmp("print", identifier)) {
        array_dependent_identifiers->depentent_identifers[array_dependent_identifiers->size++] = ip;
        struct parser_token_type_dependency *pre_type_deps = malloc(sizeof(struct parser_token_type_dependency));
        pre_type_deps->array_dependencies = calloc(1, sizeof(parser_token_type_dependency_details_t));
        pre_type_deps->size = 1;
        pre_type_deps->array_dependencies[0] = (parser_token_type_dependency_details_t) {
            .dependency = PARSER_INT_LIKE,
            .position_rel_to_token = -1
        };

        parser_token_t token = {
            .data = {0},
            .params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_PRINT,
            .pre_op_type_dependencies = pre_type_deps,
            .post_op_type_dependencies = NULL,
            .pre_operations_dependencies = NULL,
            .post_operations_dependencies = NULL
        };
        return token;
    }
    else if(0 == strcmp("if", identifier)) {
        array_dependent_identifiers->depentent_identifers[array_dependent_identifiers->size++] = ip;
        struct parser_token_operation_dependency *pos_op_deps = malloc(sizeof(struct parser_token_operation_dependency));
        struct parser_token_operation_dependency *opt_op_deps = malloc(sizeof(struct parser_token_operation_dependency));
        struct parser_token_type_dependency *pre_type_deps = malloc(sizeof(struct parser_token_type_dependency));

        pos_op_deps->array_dependencies = calloc(1, sizeof(parser_token_operation_dependency_details_t));
        opt_op_deps->array_dependencies = calloc(1, sizeof(parser_token_operation_dependency_details_t));
        pre_type_deps->array_dependencies = calloc(1, sizeof(parser_token_type_dependency_details_t));

        pos_op_deps->size = 1;
        opt_op_deps->size = 1;
        pre_type_deps->size = 1;

        pos_op_deps->array_dependencies[0] = (parser_token_operation_dependency_details_t) { .dependency = PARSER_END, .position_rel_to_token = 0 };
        opt_op_deps->array_dependencies[0] = (parser_token_operation_dependency_details_t) { .dependency = PARSER_ELSE, .position_rel_to_token = 0 };
        pre_type_deps->array_dependencies[0] = (parser_token_type_dependency_details_t) { .dependency = PARSER_BOOL, .position_rel_to_token = -1 };

        parser_token_t token = {
            .data = {0},
            .params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_IF,
            .pre_op_type_dependencies = pre_type_deps,
            .post_op_type_dependencies = NULL,
            .optional_operations = opt_op_deps,
            .pre_operations_dependencies = NULL,
            .post_operations_dependencies = pos_op_deps,
        };
        return token;
    }
    else if(0 == strcmp("end", identifier)) {
        array_dependent_identifiers->depentent_identifers[array_dependent_identifiers->size++] = ip;
        struct parser_token_operation_dependency *pre_op_deps = malloc(sizeof(struct parser_token_operation_dependency));
        pre_op_deps->array_dependencies = calloc(1, sizeof(parser_token_operation_dependency_details_t));
        pre_op_deps->array_dependencies[0] = (parser_token_operation_dependency_details_t) { .dependency = PARSER_IF, .position_rel_to_token = 0 };
        pre_op_deps->size = 1;

        parser_token_t token = {
            .data = {0},
            .params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_END,
            .pre_op_type_dependencies = NULL,
            .post_op_type_dependencies = NULL,
            .optional_operations = NULL,
            .pre_operations_dependencies = pre_op_deps,
            .post_operations_dependencies = NULL,
        };
        return token;
    }
    fprintf(stderr, "undefined identifier '%s'\n", identifier);
    exit(ERR_UNKNOW_IDENTIFIER);
}


/**
 * @brief Validates if `token` satifies the a dependency of `dependent_token` for types
 *
 * @param array_generated_tokens Array of all tokens.
 * @param index current index of the tokens array.
 * @param dependent_token token to match dependencies.
 * @param token token to verify the dependency.
 * 
 * @return number of dependencies satisfied.
 */
size_t type_dependency_rate(const parser_token_t *const array_generated_tokens, const size_t index, const parser_token_t *const dependent_token, const parser_token_t *const token) {
    size_t dependencies_found = 0;
    switch(dependent_token->pre_op_type_dependencies->array_dependencies[index-1].dependency) {
    case PARSER_INT_LIKE: {
        if(IS_INT_LIKE(token->type)) {
            dependencies_found++;
        }
        break;
    }
    default: {
        if(token->type == dependent_token->pre_op_type_dependencies->array_dependencies[index-1].dependency) {
            dependencies_found++;
        }
    }
    }
    return dependencies_found;
}

/**
 * @brief Validates if `token` satifies the a dependency of `dependent_token` for operations
 *
 * @param array_generated_tokens Array of all tokens.
 * @param index current index of the tokens array.
 * @param dependent_token token to match dependencies.
 * @param token token to verify the dependency.
 * 
 * @return number of dependencies satisfied.
 */
size_t operation_dependency_rate(const parser_token_t *const array_generated_tokens, const size_t index, const parser_token_t *const dependent_token, const parser_token_t *const token) {
    size_t dependencies_found = 0;
    if(token->operation == dependent_token->pre_operations_dependencies->array_dependencies[index-1].dependency) {
        dependencies_found++;
    }
    return dependencies_found;
}