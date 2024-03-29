#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../pkg/collections/vector/vector.h"
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

    token.array_params = NULL;
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
static parser_token_t construct_operator(const size_t ip, const char *identifier, struct dependent_identifiers *array_dependent_identifiers) {
    parser_token_t token;
    switch (identifier[0]) {
    case '-': {
        array_dependent_identifiers->depentent_identifers[array_dependent_identifiers->size++] = ip;
        struct parser_token_type_dependency* array_p_type_deps = malloc(sizeof(struct parser_token_type_dependency));
        array_p_type_deps->size = 2;
        array_p_type_deps->array_dependencies = calloc(2, sizeof(parser_token_type_dependency_details_t));
        array_p_type_deps->array_dependencies[0] = (parser_token_type_dependency_details_t) {
            .dependency = PARSER_INT_LIKE,
            .position_rel_to_token = -1
        };
        array_p_type_deps->array_dependencies[1] = (parser_token_type_dependency_details_t) {
            .dependency = PARSER_INT_LIKE,
            .position_rel_to_token = -2
        };
        token = (parser_token_t) {
            .type = PARSER_INT_LIKE,
            .operation = PARSER_SUB,
            .array_pre_op_type_dependencies = array_p_type_deps
        };
        break;
    }
    case '+': {
        array_dependent_identifiers->depentent_identifers[array_dependent_identifiers->size++] = ip;
        struct parser_token_type_dependency* array_p_type_deps = malloc(sizeof(struct parser_token_type_dependency));
        array_p_type_deps->size = 2;
        array_p_type_deps->array_dependencies = calloc(2, sizeof(parser_token_type_dependency_details_t));
        array_p_type_deps->array_dependencies[0] = (parser_token_type_dependency_details_t) {
            .dependency = PARSER_INT_LIKE,
            .position_rel_to_token = -1
        };
        array_p_type_deps->array_dependencies[1] = (parser_token_type_dependency_details_t) {
            .dependency = PARSER_INT_LIKE,
            .position_rel_to_token = -2
        };

        token = (parser_token_t) {
            .type = PARSER_INT_LIKE,
            .operation = PARSER_SUM,
            .array_pre_op_type_dependencies = array_p_type_deps
        };
        break;
    }
    case '=': {
        array_dependent_identifiers->depentent_identifers[array_dependent_identifiers->size++] = ip;
        struct parser_token_type_dependency* array_p_type_deps = malloc(sizeof(struct parser_token_type_dependency));
        array_p_type_deps->size = 1;
        array_p_type_deps->array_dependencies = calloc(1, sizeof(parser_token_type_dependency_details_t));
        array_p_type_deps->array_dependencies[0] = (parser_token_type_dependency_details_t) {
            .dependency = PARSER_INT_LIKE,
            .position_rel_to_token = -1
        };
        token = (parser_token_t) {
            .type = PARSER_BOOL,
            .operation = PARSER_EQ,
            .array_pre_op_type_dependencies = array_p_type_deps
        };
        break;
    }
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
            .array_params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_PRINT,
            .array_pre_op_type_dependencies = pre_type_deps,
            .array_pos_op_type_dependencies = NULL,
            .array_pre_operations_dependencies = NULL,
            .array_pos_operations_dependencies = NULL
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
            .array_params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_IF,
            .array_pre_op_type_dependencies = pre_type_deps,
            .array_pos_op_type_dependencies = NULL,
            .array_optional_operations = opt_op_deps,
            .array_pre_operations_dependencies = NULL,
            .array_pos_operations_dependencies = pos_op_deps,
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
            .array_params = NULL,
            .type = PARSER_VOID,
            .operation = PARSER_END,
            .array_pre_op_type_dependencies = NULL,
            .array_pos_op_type_dependencies = NULL,
            .array_optional_operations = NULL,
            .array_pre_operations_dependencies = pre_op_deps,
            .array_pos_operations_dependencies = NULL,
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
void pre_type_dependency_rate(vector_t *array_generated_tokens, const size_t index, parser_token_t *dependent_token, parser_token_t *token) {
    switch(dependent_token->array_pre_op_type_dependencies->array_dependencies[index-1].dependency) {
    case PARSER_INT_LIKE: {
        if((IS_INT_LIKE(token->type) || PRODUCE_INT_LIKE(token->operation)) &&
           dependent_token->array_pre_op_type_dependencies->array_dependencies[index-1].satisfied == false) {
            dependent_token->array_pre_op_type_dependencies->array_dependencies[index-1].satisfied  = true;
        }
        break;
    }
    default: {
        if(token->type == dependent_token->array_pre_op_type_dependencies->array_dependencies[index-1].dependency &&
           dependent_token->array_pre_op_type_dependencies->array_dependencies[index-1].satisfied == false) {
            dependent_token->array_pre_op_type_dependencies->array_dependencies[index-1].satisfied  = true;
        }
    }
    }
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
void pos_type_dependency_rate(const size_t index, parser_token_t *dependent_token, parser_token_t *token) {
    switch(dependent_token->array_pos_op_type_dependencies->array_dependencies[index].dependency) {
    case PARSER_INT_LIKE: {
        if((IS_INT_LIKE(token->type) || PRODUCE_INT_LIKE(token->operation)) &&
           dependent_token->array_pos_op_type_dependencies->array_dependencies[index].satisfied == false) {
            dependent_token->array_pos_op_type_dependencies->array_dependencies[index].satisfied = true;
        }
        break;
    }
    default: {
        if(token->type == dependent_token->array_pos_op_type_dependencies->array_dependencies[index].dependency &&
           dependent_token->array_pos_op_type_dependencies->array_dependencies[index].satisfied == false) {
            dependent_token->array_pos_op_type_dependencies->array_dependencies[index].satisfied = true;
        }
    }
    }
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
void pre_operation_dependency_rate(const size_t token_index, const size_t dep_index, parser_token_t *dependent_token, parser_token_t *token) {
    if(token->operation == dependent_token->array_pre_operations_dependencies->array_dependencies[dep_index-1].dependency &&
       dependent_token->array_pre_operations_dependencies->array_dependencies[dep_index-1].satisfied == false) {
        dependent_token->array_pre_operations_dependencies->array_dependencies[dep_index-1].satisfied = true;
    }
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
void pos_operation_dependency_rate(const size_t token_index, const size_t dep_index, parser_token_t *dependent_token, parser_token_t *token) {
    if(token->operation == dependent_token->array_pos_operations_dependencies->array_dependencies[dep_index].dependency &&
       dependent_token->array_pos_operations_dependencies->array_dependencies[dep_index].satisfied == false) {
        dependent_token->array_pos_operations_dependencies->array_dependencies[dep_index].satisfied = true;
    }
}