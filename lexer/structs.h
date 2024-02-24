#ifndef SOF_LEXER_STRUCTS_H
#define SOF_LEXER_STRUCTS_H

#include <stdint.h>
#include <stddef.h>

/*
 * Struct: lexer_file
 * ----------------------------
 * Holds the amount of bytes, content, and a possible error of a file
 */
struct lexer_file {
    size_t bytes_sz;
    uint8_t error_code;
    char *content;
};

#endif  // SOF_LEXER_STRUCTS_H