#include <stdio.h>
#include <stdlib.h>

#include "win64.h"

#include "../../../structs.h"

void inline compile_win64(struct parser_array_token parser_tokens) {
    FILE *file = fopen(ASM_OUT_DIR, "w");

    fprintf(file, "%s", "include C:\\masm64\\include64\\masm64rt.inc\n");
    fprintf(file, "%s", ".CODE\n");
    fprintf(file, "%s", "main proc\n");
    for(size_t ip = 0; ip < parser_tokens.size; ++ip) {
        const parser_token_t token = parser_tokens.array[ip];
        
        switch(token.operation) {
        case PARSER_PUSH: {
            char* command = malloc(sizeof(char)*20);
            sprintf(command, "push %lld\n", token.data.u64_value);
            fprintf(file, "     %s", command);
            free(command);
            break;
        }
        case PARSER_SUM: {
            fprintf(file, "     pop rax\n");
            fprintf(file, "     pop rbx\n");
            fprintf(file, "     add rax, rbx\n");
            fprintf(file, "     push rax\n");
            break;
        }
        case PARSER_SUB: {
            fprintf(file, "     pop rax\n");
            fprintf(file, "     pop rbx\n");
            fprintf(file, "     sub rax, rbx\n");
            fprintf(file, "     push rax\n");
            break;
        }
        }
    }
    fprintf(file, "%s", "\tpop rax\n");
    fprintf(file, "%s", "\tconout str$(rax)\n");
    fprintf(file, "%s", "\tinvoke ExitProcess, 0\n");
    fprintf(file, "%s", "main endp\n");
    fprintf(file, "%s", "end\n");

    fclose(file);

    system(MASM_CMD);
    system(WIN_CLEAR_COMPILATION_FILES);
}