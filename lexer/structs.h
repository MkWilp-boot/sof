#ifndef CORTH_FILE_H
#define CORTH_FILE_H

#include <stddef.h>
#include <stdint.h>

/*
 * Struct: lexer_file
 * ----------------------------
 * Holds the amount of bytes, content, and a possible error of a file
 */
struct lexer_file {
    size_t bytes_sz;
    uint8_t error_code;
    char* content;
};

/*
 * Struct: lexer_file_identifiers
 * ----------------------------
 * Holds all identifiers of a file, it's amount and a possible error
 */
struct lexer_file_identifiers {
    char** identifiers;
    uint8_t error_code;
    size_t size;
};

#endif  // CORTH_FILE_H