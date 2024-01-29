#include <stdio.h>
#include <stdlib.h>

#include "pkg/error_codes.h"

#include "lexer/lexer.h"
#include "lexer/structs.h"

#include "parser/parser.h"
#include "parser/structs.h"

int main2() {
    char* converted_str;
    char* str = "10";
    long result = strtol(str, &converted_str, 10);

    printf("%ld | %d\n", result, *converted_str);

    printf("converted_str == str: %d\n", converted_str == str);

    return 0;
}

int main() {
    struct lexer_file file = lexer_read("main.sof");
    if(0 != file.error_code) {
        return file.error_code;
    }
    struct lexer_file_identifiers file_identifiers = lexer_build_identifiers(file);
    if(0 != file_identifiers.error_code) {
        return file_identifiers.error_code;
    }

    parser_tokenize(&file_identifiers);
    
    return 0;
}