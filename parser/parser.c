#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../pkg/collections/vector/vector.h"

#include "../lexer/structs.h"
#include "../lexer/lexer.h"

#include "parser.h"
#include "structs.h"
#include "helper.h"

#define DEPENDENT_IDENTIFIERS_SIZE 32000

static inline void validate_token_pre_type_dependencies(vector_t *array_generated_tokens, const struct dependent_identifiers *const depentent_identifiers) {
    for(size_t i = 0; i < depentent_identifiers->size; i++) {
        uint32_t dependent_token_ip = depentent_identifiers->depentent_identifers[i];
        parser_token_t *dependent_token = vec_get(array_generated_tokens, dependent_token_ip);

        if(NULL != dependent_token->array_pre_op_type_dependencies && dependent_token->array_pre_op_type_dependencies->size > 0) {
            for(size_t j = dependent_token->array_pre_op_type_dependencies->size; j > 0; j--) {
                int8_t required_dependencie_position = dependent_token->array_pre_op_type_dependencies->array_dependencies[j-1].position_rel_to_token;
                if(required_dependencie_position < 0) {
                    int index = dependent_token_ip+required_dependencie_position;
                    if(index < 0) {
                        fprintf(stderr, "%s", "ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED\n");
                        exit(ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED);
                    }
                    parser_token_t *token = vec_get(array_generated_tokens, index);
                    pre_type_dependency_rate(array_generated_tokens, j, dependent_token, token);
                }
                else {
                    for(size_t k = dependent_token_ip; k > 0; k--) {
                        parser_token_t *token = vec_get(array_generated_tokens, k-1);
                        pre_type_dependency_rate(array_generated_tokens, j, dependent_token, token);
                    }
                }
            }
            size_t dependencies_satisfied = 0;
            for(size_t j = dependent_token->array_pre_op_type_dependencies->size; j > 0; j--) {
                if(dependent_token->array_pre_op_type_dependencies->array_dependencies[j-1].satisfied) {
                    dependencies_satisfied++;
                }
            }
            if(dependencies_satisfied != dependent_token->array_pre_op_type_dependencies->size) {
                printf("dependent_token->operation %d\n", dependent_token->operation);
                fprintf(stderr, "%s", "ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED\n");
                exit(ERR_TOKEN_PRE_TYPE_DEPENDENCIES_NOT_SATISFIED);
            }
        }
    }
}

static inline void validate_token_pos_operation_dependencies(vector_t *array_generated_tokens, const struct dependent_identifiers *const depentent_identifiers, const size_t max_array_tokens_size) {
    for(size_t i = 0; i < depentent_identifiers->size; i++) {
        uint32_t dependent_token_ip = depentent_identifiers->depentent_identifers[i];
        parser_token_t *dependent_token = vec_get(array_generated_tokens, dependent_token_ip);
        if(NULL != dependent_token->array_pos_operations_dependencies && dependent_token->array_pos_operations_dependencies->size > 0) {
            for(size_t j = 0; j < dependent_token->array_pos_operations_dependencies->size; j++) {
                int8_t required_dependencie_position = dependent_token->array_pos_operations_dependencies->array_dependencies[j].position_rel_to_token;
                if(required_dependencie_position > 0) {
                    int32_t token_index = dependent_token_ip-required_dependencie_position;
                    parser_token_t *token = vec_get(array_generated_tokens, token_index);
                    pos_operation_dependency_rate(token_index, j, dependent_token, token);
                }
                else {
                    for(size_t k = dependent_token_ip; k < max_array_tokens_size; k++) {
                        parser_token_t *token = vec_get(array_generated_tokens, k);
                        pos_operation_dependency_rate(k, j, dependent_token, token);
                    }
                }
            }

            size_t dependencies_satisfied = 0;
            for(size_t j = 0; j < dependent_token->array_pos_operations_dependencies->size; j++) {
                if(dependent_token->array_pos_operations_dependencies->array_dependencies[j].satisfied) {
                    dependencies_satisfied++;
                }
            }
            if(dependencies_satisfied != dependent_token->array_pos_operations_dependencies->size) {
                fprintf(stderr, "%s", "ERR_TOKEN_POS_OPERATION_DEPENDENCIES_NOT_SATISFIED\n");
                exit(ERR_TOKEN_POS_OPERATION_DEPENDENCIES_NOT_SATISFIED);
            }
        }
    }
}

static inline void validate_token_pre_operation_dependencies(vector_t *array_generated_tokens, const struct dependent_identifiers *const depentent_identifiers) {
    for(size_t i = 0; i < depentent_identifiers->size; i++) {
        uint32_t dependent_token_ip = depentent_identifiers->depentent_identifers[i];
        parser_token_t *dependent_token = vec_get(array_generated_tokens, dependent_token_ip);

        if(NULL != dependent_token->array_pre_operations_dependencies && dependent_token->array_pre_operations_dependencies->size > 0) {
            for(size_t j = dependent_token->array_pre_operations_dependencies->size; j > 0; j--) {
                int8_t required_dependencie_position = dependent_token->array_pre_operations_dependencies->array_dependencies[j-1].position_rel_to_token;
                if(required_dependencie_position < 0) {
                    int32_t token_index = dependent_token_ip+required_dependencie_position;
                    parser_token_t *token = vec_get(array_generated_tokens, token_index);
                    pre_operation_dependency_rate(token_index, j, dependent_token, token);
                }
                else {
                    for(size_t k = dependent_token_ip; k > 0; k--) {
                        parser_token_t *token = vec_get(array_generated_tokens, k-1);
                        pre_operation_dependency_rate(k-1, j, dependent_token, token);
                    }
                }
            }
            size_t dependencies_satisfied = 0;
            for(size_t j = dependent_token->array_pre_operations_dependencies->size; j > 0; j--) {
                if(dependent_token->array_pre_operations_dependencies->array_dependencies[j-1].satisfied) {
                    dependencies_satisfied++;
                }
            }

            if(dependencies_satisfied != dependent_token->array_pre_operations_dependencies->size) {
                fprintf(stderr, "%s", "ERR_TOKEN_PRE_OPERATION_DEPENDENCIES_NOT_SATISFIED\n");
                exit(ERR_TOKEN_PRE_OPERATION_DEPENDENCIES_NOT_SATISFIED);
            }
        }
    }
}

/**
 * @brief link tokens accross the `array_tokens` argument
 *
 * @attention This function modifies `array_tokens`
 * 
 * @param array_tokens Every token generated
 */
static inline void cross_reference_tokens(vector_t *array_tokens) {
    struct rel_token_index {
        parser_token_t *token;
        size_t index;
        bool used;
    };

    vector_t relation_tokens = vec_new(sizeof(struct rel_token_index));
    for(size_t i = 0; i < array_tokens->len; i++) {
        parser_token_t *token = vec_get(array_tokens, i);
        switch(token->operation) {
        case PARSER_IF: {
                struct rel_token_index relational_token = {
                    .index = i,
                    .token = token,
                    .used = false
                };
                vec_add(&relation_tokens, &relational_token);
                break;
            }
        case PARSER_END: {
            uint32_t offset = 1;
            struct rel_token_index *passed_token = vec_get(&relation_tokens, relation_tokens.len-offset);
            for(; passed_token->used; offset++) {
                passed_token = vec_get(&relation_tokens, relation_tokens.len-offset);
            }
            if(!CAN_LINK_TO_END(passed_token)) {
                fprintf(stderr, "Cannot link '%d' to PARSER_END\n", passed_token->token->operation);
                exit(ERR_CANNOT_LINK_TOKEN_BY_OP);
            }
            if(0 == passed_token->token->linked_token.elements_size) {
                passed_token->token->linked_token = vec_new(sizeof(parser_token_t));
            }
            vec_add(&passed_token->token->linked_token, token);
            passed_token->used = true;
        }
        default:
            break;
        }
    }
}

vector_t parser_tokenize(vector_t *array_file_identifiers) {
    vector_t generated_tokens = vec_new(sizeof(parser_token_t));

    uint32_t *depentent_identifers_ptr = calloc(DEPENDENT_IDENTIFIERS_SIZE, sizeof(uint32_t));
    struct dependent_identifiers dependent_identifiers = {
        .depentent_identifers = depentent_identifers_ptr,
        .size = 0
    };

    char *discart_number_conversion;
    for(size_t i = 0; i < array_file_identifiers->len; ++i) {
        const char *const identifier = vec_get(array_file_identifiers, i);
        if(IS_UNARY_OPERATION(identifier[0], identifier[1])) {
            // identifier[0] is the sign of the identifier.
            parser_token_t token = construct_number(identifier, identifier[0]);
            token.position = i;
            vec_add(&generated_tokens, &token);
        }
        // single operators
        else if(IS_OPERATOR(identifier[0])) {
            parser_token_t token = construct_operator(i, identifier, &dependent_identifiers);
            token.position = i;
            vec_add(&generated_tokens, &token);
        }
        else if(0 <= strtol(identifier, &discart_number_conversion, 10) && *discart_number_conversion == '\0') {
            // always generates a positive numbers.
            parser_token_t token = construct_number(identifier, '+');
            token.position = i;
            vec_add(&generated_tokens, &token);
            // clean our trash
            discart_number_conversion = NULL;
        }
        else {
            parser_token_t token = construct_symbol(i, identifier, &dependent_identifiers);
            token.position = i;
            vec_add(&generated_tokens, &token);
        }
    }

    validate_token_pre_type_dependencies(&generated_tokens, &dependent_identifiers);
    validate_token_pre_operation_dependencies(&generated_tokens, &dependent_identifiers);
    validate_token_pos_operation_dependencies(&generated_tokens, &dependent_identifiers, array_file_identifiers->len);

    cross_reference_tokens(&generated_tokens);
    
    free(dependent_identifiers.depentent_identifers);

    // frees file_identfiers
    lexer_free_identifiers(array_file_identifiers);
    //exit(0);
    // still working

    return generated_tokens;
}