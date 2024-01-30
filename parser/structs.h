#ifndef SOF_PARSER_STRUCTS_H
#define SOF_PARSER_STRUCTS_H

#include <stdint.h>

/**
 * @brief Enumeration of SOF instructions.
 */
enum parser_operation {
    PARSER_PUSH,
    PARSER_SUM,
    PARSER_SUB
};

/**
 * @brief Enumeration of SOF data types.
 */
enum parser_operation_type {
    PARSER_INT32,
    PARSER_UINT32,
    PARSER_INT64,
    PARSER_UINT64
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

/**
 * @brief Holds all data that identifies a valid SOF instruction token.
 */
struct parser_token {
    enum parser_operation operation;
    enum parser_operation_type type;

    union parser_data data;
    union parser_data *params;
};

#endif // SOF_PARSER_STRUCTS_H
