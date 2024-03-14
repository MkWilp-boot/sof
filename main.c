#include "pkg/error_codes.h"
#include "pkg/collections/vector/vector.h"

#include "lexer/lexer.h"
#include "lexer/structs.h"

#include "parser/parser.h"
#include "parser/structs.h"

#include "compiler/compiler.h"

int main() {
    struct lexer_file file = lexer_read("main.sof");
    if(0 != file.error_code) {
        return file.error_code;
    }
    vector_t file_identifiers = lexer_build_identifiers(file);
    vector_t parser_tokens = parser_tokenize(&file_identifiers);
    
    compile(parser_tokens);
    return 0;
}