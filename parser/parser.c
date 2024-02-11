#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../lexer/structs.h"
#include "../lexer/lexer.h"

#include "parser.h"
#include "structs.h"
#include "helper.h"

#define DEPENDENT_IDENTIFIERS_SIZE 32000

static inline void validate_token_pre_type_dependencies(const parser_token_t *const array_generated_tokens, const struct dependent_identifiers *const depentent_identifiers) {
    for(size_t i = 0; i < depentent_identifiers->size; i++) {
        uint32_t dependent_token_ip = depentent_identifiers->depentent_identifers[i];
        parser_token_t dependent_token = array_generated_tokens[dependent_token_ip];

        size_t dependencies_found = 0;

        if(NULL != dependent_token.pre_op_type_dependencies && dependent_token.pre_op_type_dependencies->size > 0) {
            for(size_t j = dependent_token.pre_op_type_dependencies->size; j > 0; j--) {
                int8_t required_dependencie_position = dependent_token.pre_op_type_dependencies->array_dependencies[j-1].position_rel_to_token;
                if(required_dependencie_position < 0) {
                    int index = dependent_token_ip+required_dependencie_position;
                    if(index < 0) {
                        fprintf(stderr, "%s", "ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED\n");
                        exit(ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED);
                    }
                    parser_token_t token = array_generated_tokens[index];
                    dependencies_found += pre_type_dependency_rate(array_generated_tokens, j, &dependent_token, &token);
                }
                else {
                    for(size_t k = dependent_token_ip; k > 0; k--) {
                        parser_token_t token = array_generated_tokens[k-1];
                        dependencies_found += pre_type_dependency_rate(array_generated_tokens, j, &dependent_token, &token);
                    }
                }
            }
            if(dependencies_found != dependent_token.pre_op_type_dependencies->size) {
                printf("dependent_token.operation %d\n", dependent_token.operation);
                fprintf(stderr, "%s", "ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED\n");
                exit(ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED);
            }
        }
    }
}

static inline void validate_token_pos_operation_dependencies(const parser_token_t *const array_generated_tokens, const struct dependent_identifiers *const depentent_identifiers, const size_t max_array_tokens_size) {
    for(size_t i = 0; i < depentent_identifiers->size; i++) {
        uint32_t dependent_token_ip = depentent_identifiers->depentent_identifers[i];
        parser_token_t dependent_token = array_generated_tokens[dependent_token_ip];
        if(NULL != dependent_token.pos_operations_dependencies && dependent_token.pos_operations_dependencies->size > 0) {
            size_t dependencies_found = 0;
            for(size_t j = 0; j < dependent_token.pos_operations_dependencies->size; j++) {
                int8_t required_dependencie_position = dependent_token.pos_operations_dependencies->array_dependencies[j].position_rel_to_token;
                if(required_dependencie_position > 0) {
                    parser_token_t token = array_generated_tokens[dependent_token_ip-required_dependencie_position];
                    dependencies_found += pos_operation_dependency_rate(array_generated_tokens, j, &dependent_token, &token);
                }
                else {
                    for(size_t k = dependent_token_ip; k < max_array_tokens_size; k++) {
                        parser_token_t token = array_generated_tokens[k];
                        dependencies_found += pos_operation_dependency_rate(array_generated_tokens, j, &dependent_token, &token);
                    }
                }
            }
            if(dependencies_found != dependent_token.pos_operations_dependencies->size) {
                fprintf(stderr, "%s", "ERR_TOKEN_POS_OPERATION_DEPENDENCIES_NOT_SATISFIED\n");
                exit(ERR_TOKEN_POS_OPERATION_DEPENDENCIES_NOT_SATISFIED);
            }
        }
    }
}

static inline void validate_token_pre_operation_dependencies(const parser_token_t *const array_generated_tokens, const struct dependent_identifiers *const depentent_identifiers) {
    for(size_t i = 0; i < depentent_identifiers->size; i++) {
        uint32_t dependent_token_ip = depentent_identifiers->depentent_identifers[i];
        parser_token_t dependent_token = array_generated_tokens[dependent_token_ip];

        size_t dependencies_found = 0;

        if(NULL != dependent_token.pre_operations_dependencies && dependent_token.pre_operations_dependencies->size > 0) {
            for(size_t j = dependent_token.pre_operations_dependencies->size; j > 0; j--) {
                int8_t required_dependencie_position = dependent_token.pre_operations_dependencies->array_dependencies[j-1].position_rel_to_token;
                if(required_dependencie_position < 0) {
                    parser_token_t token = array_generated_tokens[dependent_token_ip+required_dependencie_position];
                    dependencies_found += pre_operation_dependency_rate(array_generated_tokens, j, &dependent_token, &token);
                }
                else {
                    for(size_t k = dependent_token_ip; k > 0; k--) {
                        parser_token_t token = array_generated_tokens[k-1];
                        dependencies_found += pre_operation_dependency_rate(array_generated_tokens, j, &dependent_token, &token);
                    }
                }
            }
            
            if(dependencies_found != dependent_token.pre_operations_dependencies->size) {
                fprintf(stderr, "%s", "ERR_TOKEN_PRE_OPERATION_DEPENDENCIES_NOT_SATISFIED\n");
                exit(ERR_TOKEN_PRE_OPERATION_DEPENDENCIES_NOT_SATISFIED);
            }
        }
    }
}

struct parser_array_token parser_tokenize(struct lexer_file_identifiers *array_file_identifiers) {
    parser_token_t *generated_tokens = calloc(array_file_identifiers->size, sizeof(parser_token_t));

    uint32_t *depentent_identifers_ptr = calloc(DEPENDENT_IDENTIFIERS_SIZE, sizeof(uint32_t));
    struct dependent_identifiers dependent_identifiers = {
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
            parser_token_t token = construct_operator(i, identifier, &dependent_identifiers);
            generated_tokens[i] = token;
        }
        else if(0 <= strtol(identifier, &discart_number_conversion, 10) && *discart_number_conversion == '\0') {
            // always generates a positive numbers.
            parser_token_t token = construct_number(identifier, '+');
            generated_tokens[i] = token;

            // clean our trash
            discart_number_conversion = NULL;
        }
        else {
            parser_token_t token = construct_symbol(i, identifier, &dependent_identifiers);
            generated_tokens[i] = token;
        }
    }

    validate_token_pre_type_dependencies(generated_tokens, &dependent_identifiers);
    validate_token_pre_operation_dependencies(generated_tokens, &dependent_identifiers);
    validate_token_pos_operation_dependencies(generated_tokens, &dependent_identifiers, array_file_identifiers->size);
    
    free(dependent_identifiers.depentent_identifers);

    // frees file_identfiers
    lexer_free_identifiers(array_file_identifiers);

    // still working
    exit(0);

    struct parser_array_token parser_tokens = {
        .array = generated_tokens,
        .size = array_file_identifiers->size
    };
    return parser_tokens;
}