#include <stdio.h>
#include <stdlib.h>

#include "pkg/error_codes.h"

#include "lexer/lexer.h"
#include "lexer/structs.h"

#include "parser/parser.h"
#include "parser/structs.h"
#include "parser/compiler/sof_compiler.h"

int main() {
    struct lexer_file file = lexer_read("main.sof");
    if(0 != file.error_code) {
        return file.error_code;
    }
    struct lexer_file_identifiers file_identifiers = lexer_build_identifiers(file);
    if(0 != file_identifiers.error_code) {
        return file_identifiers.error_code;
    }

    struct parser_array_token parser_tokens = parser_tokenize(&file_identifiers);
    
    compile(parser_tokens);
    return 0;
}