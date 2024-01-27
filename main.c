#include <stdio.h>
#include <stdlib.h>

#include "pkg/error_codes.h"
#include "lexer/lexer.h"
#include "lexer/structs.h"

int main() {
    struct lexer_file file = lexer_read("main.sof");
    if(0 != file.error_code) {
        return file.error_code;
    }
    struct lexer_file_identifiers file_identifiers = lexer_build_identifiers(file);
    if(0 != file_identifiers.error_code) {
        return file_identifiers.error_code;
    }
    
    for(size_t i = 0; i < file_identifiers.size; ++i) {
        printf("'%s'\n", file_identifiers.identifiers[i]);
    }

    // cleanup
    lexer_free_identifiers(file_identifiers);
    return 0;
}