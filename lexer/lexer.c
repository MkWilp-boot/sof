#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../pkg/error_codes.h"

#include "lexer.h"
#include "structs.h"

#define FILE_BYTES_AMOUNT(bytes) bytes > 0 ? bytes + 1 : 0

struct lexer_identifier_allocation_result {
    char* identifier;
    uint8_t error_code;
};

static struct lexer_identifier_allocation_result create_identifier(struct lexer_file file, uint32_t end, uint32_t begin) {
    /*Must correct "i"nth indexing to skip/avoid spaces*/
    size_t identifer_size = ((end - 1) - begin) + 1 /*NULL terminator*/; 
    char* identifier = (char*)malloc((identifer_size + 1) * sizeof(char));

    if(NULL == identifier) {
        struct lexer_identifier_allocation_result l_file = { 
            .identifier = NULL,
            .error_code = ERR_IDENTIFIER_ALLOCATION
        };
        return l_file;
    }

    strncpy(identifier, file.content + begin, identifer_size);
    identifier[identifer_size] = '\0'; // adding NULL terminator by hand
    struct lexer_identifier_allocation_result l_file = { 
        .identifier = identifier,
        .error_code = 0
    };
    return l_file;
}

struct lexer_file lexer_read(char* file_name) {
    FILE* source_file = fopen(file_name, "r");

    if(NULL == source_file) {
        fclose(source_file);
        struct lexer_file c_file = { 
            .bytes_sz = 0,
            .content = NULL,
            .error_code = ERR_SOURCE_READ_ERROR
        };
        return c_file;
    }

    fseek(source_file, 0, SEEK_END);
    size_t file_bytes_amount = FILE_BYTES_AMOUNT(ftell(source_file));
    if(0 >= file_bytes_amount) {
        struct lexer_file c_file = { 
            .bytes_sz = 0,
            .content = NULL,
            .error_code = ERR_EMPTY_FILE
        };
        return c_file;
    }

    fseek(source_file, 0, SEEK_SET);

    char* source_content = (char*)malloc(file_bytes_amount);
    size_t file_bytes_read = fread(source_content, 1, file_bytes_amount, source_file);
    fclose(source_file);

    if(file_bytes_read != (file_bytes_amount-1)) {
        struct lexer_file c_file = { 
            .bytes_sz = 0,
            .content = NULL, 
            .error_code = ERR_FILE_READING_ERROR
        };
        return c_file;
    }

    struct lexer_file c_file = { 
        .bytes_sz = file_bytes_read,
        .content = source_content, 
        .error_code = 0
    };
    return c_file;
}

struct lexer_file_identifiers lexer_build_identifiers(struct lexer_file file) {
    size_t identifiers_size = 0;

    for(uint32_t i = 0; i < file.bytes_sz; ++i) {
        switch(file.content[i]) {
            case '\n':
            case ' ':
                identifiers_size++;
            break;
        }
    }
    identifiers_size++;

    char** identifiers = (char**)calloc(identifiers_size, sizeof(char*));
    if(NULL == identifiers) {
        struct lexer_file_identifiers l_file = { 
            .size = 0,
            .identifiers = NULL,
            .error_code = ERR_IDENTIFIERS_ALLOCATION
        };
        return l_file;
    }

    uint32_t last_chopped_str_position = 0;
    uint32_t current_identifier_position = 0;

    for(uint32_t i = 0; i < file.bytes_sz; ++i) {
        switch(file.content[i]) {
            case '\n':
            case ' ': {
                struct lexer_identifier_allocation_result identifiers_result = create_identifier(file, i, last_chopped_str_position);
                if(0 != identifiers_result.error_code) {
                    struct lexer_file_identifiers l_file = { 
                        .size = 0,
                        .identifiers = NULL,
                        .error_code = identifiers_result.error_code
                    };
                    return l_file;
                }

                identifiers[current_identifier_position] = identifiers_result.identifier;
                current_identifier_position++;
                last_chopped_str_position = i+1;
            }
            break;
        }
    }

    //last identifier will be missed unless
    struct lexer_identifier_allocation_result identifiers_result = create_identifier(file, file.bytes_sz, last_chopped_str_position);
    if(0 != identifiers_result.error_code) {
        struct lexer_file_identifiers l_file = { 
            .size = 0,
            .identifiers = NULL,
            .error_code = identifiers_result.error_code
        };
        return l_file;
    }
    identifiers[current_identifier_position] = identifiers_result.identifier;

    // free file.content as mentioned
    free(file.content);
    file.content = NULL;

    struct lexer_file_identifiers l_file = { 
        .size = identifiers_size,
        .identifiers = identifiers,
        .error_code = 0
    };
    return l_file;
}

void inline lexer_free_identifiers(struct lexer_file_identifiers* identifiers) {
    if(NULL == identifiers) {
        return;
    }
    for(size_t i = 0; i < identifiers->size; ++i) {
        if(NULL != identifiers->identifiers[i]) {
            free(identifiers->identifiers[i]);
            identifiers->identifiers[i] = NULL;
        }
    }
    if(NULL != identifiers->identifiers) {
        free(identifiers->identifiers);
        identifiers->identifiers = NULL;
    }
    identifiers = NULL;
}
