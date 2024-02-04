#include <stdio.h>
#include <stdlib.h>

#include "../../parser/structs.h"

#include "linux.h"

void compile_linux(struct parser_array_token parser_tokens) {
    FILE *file = fopen(ASM_OUT_DIR, "w");

    fprintf(file, "BITS 64\n");
    fprintf(file, "global _start\n");
    fprintf(file, "section .text\n");
    fprintf(file, "_start:\n");
    for(size_t ip = 0; ip < parser_tokens.size; ++ip) {
        const parser_token_t token = parser_tokens.array[ip];
        
        switch(token.operation) {
        case PARSER_PUSH: {
            char* command = malloc(sizeof(char)*20);
            sprintf(command, "push      %ld\n", token.data.u64_value);
            fprintf(file, "     %s", command);
            free(command);
            break;
        }
        case PARSER_SUM: {
            fprintf(file, "     pop     rax\n");
            fprintf(file, "     pop     rbx\n");
            fprintf(file, "     add     rax, rbx\n");
            fprintf(file, "     push    rax\n");
            break;
        }
        case PARSER_SUB: {
            fprintf(file, "     pop     rax\n");
            fprintf(file, "     pop     rbx\n");
            fprintf(file, "     sub     rax, rbx\n");
            fprintf(file, "     push    rax\n");
            break;
        }
        case PARSER_EQ: {
            fprintf(file, "     pop     rax\n");
            fprintf(file, "     pop     rbx\n");
            fprintf(file, "     cmp     rax, rbx\n");
            fprintf(file, "     mov     rax, 0\n");
            fprintf(file, "     mov     rbx, 1\n");
            fprintf(file, "     cmove   rax, rbx\n");
            fprintf(file, "     push    rax\n");
            break;
        }
        }
    }

    fprintf(file, "     pop     rax\n");
    fprintf(file, "     mov     rdi, rax\n");
    fprintf(file, "     mov     rax, 0x3c\n");
    fprintf(file, "     syscall\n");

    fclose(file);

    system(NASM_CMD);
    system(LINKER_CMD);
}