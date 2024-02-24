#ifndef SOF_PARSER_STRUCTS_H
#define SOF_PARSER_STRUCTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Enumeration of SOF instructions.
 */
enum parser_operation {
    PARSER_PUSH,
    PARSER_SUM,
    PARSER_SUB,
    PARSER_EQ,
    PARSER_PRINT,
    PARSER_IF,
    PARSER_END,
    PARSER_ELSE
};

/**
 * @brief Enumeration of SOF data types.
 */
enum parser_operation_type {
    PARSER_INT32,
    PARSER_UINT32,
    PARSER_INT64,
    PARSER_UINT64,
    PARSER_INT_LIKE,
    PARSER_BOOL,
    PARSER_VOID
};

/**
 * @brief Union of SOF data types.
 */
union parser_data {
    float f_value;

    int32_t i32_value;
    uint32_t u32_value;

    int64_t i64_value;
    uint64_t u64_value;
};

typedef struct {
    enum parser_operation dependency;
    bool satisfied;
    int8_t position_rel_to_token;
} parser_token_operation_dependency_details_t;

typedef struct {
    enum parser_operation_type dependency;
    bool satisfied;
    int8_t position_rel_to_token;
} parser_token_type_dependency_details_t;

struct parser_token_operation_dependency {
    parser_token_operation_dependency_details_t *array_dependencies;
    size_t size;
};

struct parser_token_type_dependency {
    parser_token_type_dependency_details_t *array_dependencies;
    size_t size;
};

/**
 * @brief Holds all data that identifies a valid SOF instruction token.
 */
typedef struct parser_token {
    enum parser_operation operation;
    enum parser_operation_type type;

    union parser_data data;
    union parser_data *array_params;

    struct parser_token_type_dependency *array_pre_op_type_dependencies;
    struct parser_token_type_dependency *array_pos_op_type_dependencies;

    struct parser_token_operation_dependency *array_optional_operations;

    struct parser_token_operation_dependency *array_pre_operations_dependencies;
    struct parser_token_operation_dependency *array_pos_operations_dependencies;
} parser_token_t;

#endif // SOF_PARSER_STRUCTS_H
