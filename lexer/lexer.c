#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../pkg/error_codes.h"
#include "../pkg/collections/vector/vector.h"

#include "lexer.h"
#include "structs.h"

static char *split_file_delimiters = " \t\r\n";

struct lexer_file lexer_read(const char *const file_name) {
    FILE *source_file = fopen(file_name, "rb");

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
    size_t file_bytes_amount = ftell(source_file);
    if(0 >= file_bytes_amount) {
        struct lexer_file c_file = { 
            .bytes_sz = 0,
            .content = NULL,
            .error_code = ERR_EMPTY_FILE
        };
        return c_file;
    }

    rewind(source_file);

    char *const source_content = malloc(file_bytes_amount+1);
    size_t file_bytes_read = fread(source_content, 1, file_bytes_amount, source_file);
    source_content[file_bytes_amount] = '\0';

    fclose(source_file);

    if(file_bytes_read != file_bytes_amount) {
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

vector_t lexer_build_identifiers(struct lexer_file file) {
    vector_t vec = vec_new(sizeof(char*));
    char *fcontent = malloc(strlen(file.content)*sizeof(char)+1);
    strcpy(fcontent, file.content);
    
    char *tmp_token = strtok(fcontent, split_file_delimiters);
    while (tmp_token != NULL) {
        size_t token_str_size = (strlen(tmp_token)*sizeof(char))+1;
        char *token_copy = malloc(token_str_size);
        strcpy(token_copy, tmp_token);
        
        vec_add(&vec, token_copy);
        tmp_token = strtok(NULL, split_file_delimiters);
    }

    free(file.content);
    file.content = NULL;
    return vec;
}

void inline lexer_free_identifiers(vector_t *identifiers) {
    if(NULL == identifiers) {
        return;
    }
    if(NULL != identifiers->ptr) {
        free(identifiers->ptr);
        identifiers->ptr = NULL;
    }
    identifiers = NULL;
}
