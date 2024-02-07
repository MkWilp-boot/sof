#ifndef SOF_PARSER_STRUCTS_H
#define SOF_PARSER_STRUCTS_H

#include <stdint.h>

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

struct parser_token_operation_dependency {
    enum parser_operation *dependencies;
    size_t size;
};

struct parser_token_type_dependency {
    enum parser_operation_type *dependencies;
    size_t size;
};

/**
 * @brief Holds all data that identifies a valid SOF instruction token.
 */
typedef struct parser_token {
    enum parser_operation operation;
    enum parser_operation_type type;

    union parser_data data;
    union parser_data *params;

    struct parser_token_type_dependency *pre_op_type_dependencies;
    struct parser_token_type_dependency *post_op_type_dependencies;

    struct parser_token_operation_dependency *optional_operations;

    struct parser_token_operation_dependency *pre_operations_dependencies;
    struct parser_token_operation_dependency *post_operations_dependencies;
} parser_token_t;

struct parser_array_token {
    size_t size;
    parser_token_t* array;
};

#endif // SOF_PARSER_STRUCTS_H
