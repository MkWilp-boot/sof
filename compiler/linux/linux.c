#include <stdio.h>
#include <stdlib.h>

#include "../../parser/structs.h"
#include "../../pkg/error_codes.h"
#include "../../pkg/collections/vector/vector.h"

#include "linux.h"

void compile_linux(vector_t parser_tokens) {
    FILE *file = fopen(ASM_OUT_DIR, "w");

    fprintf(file, "BITS 64\n\n");
    fprintf(file, "global _start\n\n");
    fprintf(file, "section .text\n");

    // Generated asm function, do not modify
    fprintf(file, "%s", "print:\n");
    fprintf(file, "%s", "    push    rbp\n");
    fprintf(file, "%s", "    mov     rbp, rsp\n");
    fprintf(file, "%s", "    sub     rsp, 64\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-56], rdi\n");
    fprintf(file, "%s", "    lea     rax, [rbp-48]\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-8], rax\n");
    fprintf(file, "%s", "    mov     rax, QWORD [rbp-56]\n");
    fprintf(file, "%s", "    shr     rax, 63\n");
    fprintf(file, "%s", "    movzx   eax, al\n");
    fprintf(file, "%s", "    mov     DWORD [rbp-20], eax\n");
    fprintf(file, "%s", "    cmp     DWORD [rbp-20], 0\n");
    fprintf(file, "%s", "    je      .L2\n");
    fprintf(file, "%s", "    mov     rax, QWORD [rbp-8]\n");
    fprintf(file, "%s", "    lea     rdx, [rax+1]\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-8], rdx\n");
    fprintf(file, "%s", "    mov     BYTE [rax], 45\n");
    fprintf(file, "%s", "    neg     QWORD [rbp-56]\n");
    fprintf(file, "%s", ".L2:\n");
    fprintf(file, "%s", "    cmp     QWORD [rbp-56], 0\n");
    fprintf(file, "%s", "    jne     .L3\n");
    fprintf(file, "%s", "    mov     rax, QWORD [rbp-8]\n");
    fprintf(file, "%s", "    lea     rdx, [rax+1]\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-8], rdx\n");
    fprintf(file, "%s", "    mov     BYTE [rax], 48\n");
    fprintf(file, "%s", "    jmp     .L4\n");
    fprintf(file, "%s", ".L3:\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-16], 1\n");
    fprintf(file, "%s", "    jmp     .L5\n");
    fprintf(file, "%s", ".L6:\n");
    fprintf(file, "%s", "    mov     rdx, QWORD [rbp-16]\n");
    fprintf(file, "%s", "    mov     rax, rdx\n");
    fprintf(file, "%s", "    sal     rax, 2\n");
    fprintf(file, "%s", "    add     rax, rdx\n");
    fprintf(file, "%s", "    add     rax, rax\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-16], rax\n");
    fprintf(file, "%s", ".L5:\n");
    fprintf(file, "%s", "    mov     rcx, QWORD [rbp-56]\n");
    fprintf(file, "%s", "    mov     rdx, 7378697629483820647\n");
    fprintf(file, "%s", "    mov     rax, rcx\n");
    fprintf(file, "%s", "    imul    rdx\n");
    fprintf(file, "%s", "    mov     rax, rdx\n");
    fprintf(file, "%s", "    sar     rax, 2\n");
    fprintf(file, "%s", "    sar     rcx, 63\n");
    fprintf(file, "%s", "    mov     rdx, rcx\n");
    fprintf(file, "%s", "    sub     rax, rdx\n");
    fprintf(file, "%s", "    cmp     QWORD [rbp-16], rax\n");
    fprintf(file, "%s", "    jle     .L6\n");
    fprintf(file, "%s", "    jmp     .L7\n");
    fprintf(file, "%s", ".L8:\n");
    fprintf(file, "%s", "    mov     rax, QWORD [rbp-56]\n");
    fprintf(file, "%s", "    cqo\n");
    fprintf(file, "%s", "    idiv    QWORD [rbp-16]\n");
    fprintf(file, "%s", "    add     eax, 48\n");
    fprintf(file, "%s", "    mov     ecx, eax\n");
    fprintf(file, "%s", "    mov     rax, QWORD [rbp-8]\n");
    fprintf(file, "%s", "    lea     rdx, [rax+1]\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-8], rdx\n");
    fprintf(file, "%s", "    mov     BYTE [rax], cl\n");
    fprintf(file, "%s", "    mov     rax, QWORD [rbp-56]\n");
    fprintf(file, "%s", "    cqo\n");
    fprintf(file, "%s", "    idiv    QWORD [rbp-16]\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-56], rdx\n");
    fprintf(file, "%s", "    mov     rcx, QWORD [rbp-16]\n");
    fprintf(file, "%s", "    mov     rdx, 7378697629483820647\n");
    fprintf(file, "%s", "    mov     rax, rcx\n");
    fprintf(file, "%s", "    imul    rdx\n");
    fprintf(file, "%s", "    mov     rax, rdx\n");
    fprintf(file, "%s", "    sar     rax, 2\n");
    fprintf(file, "%s", "    sar     rcx, 63\n");
    fprintf(file, "%s", "    mov     rdx, rcx\n");
    fprintf(file, "%s", "    sub     rax, rdx\n");
    fprintf(file, "%s", "    mov     QWORD [rbp-16], rax\n");
    fprintf(file, "%s", ".L7:\n");
    fprintf(file, "%s", "    cmp     QWORD [rbp-16], 0\n");
    fprintf(file, "%s", "    jg      .L8\n");
    fprintf(file, "%s", ".L4:\n");
    fprintf(file, "%s", "    mov     rax, QWORD [rbp-8]\n");
    fprintf(file, "%s", "    mov     BYTE [rax], 0\n");
    fprintf(file, "%s", "    lea     rax, [rbp-48]\n");
    fprintf(file, "%s", "    mov     rdx, QWORD [rbp-8]\n");
    fprintf(file, "%s", "    sub     rdx, rax\n");
    fprintf(file, "%s", "    lea     rax, [rbp-48]\n");
    fprintf(file, "%s", "    mov     rsi, rax\n");
    fprintf(file, "%s", "    mov     rax, 1\n");
    fprintf(file, "%s", "    mov     edi, 1\n");
    fprintf(file, "%s", "    syscall\n");
    fprintf(file, "%s", "    nop\n");
    fprintf(file, "%s", "    leave\n");
    fprintf(file, "%s", "    ret\n\n");

    fprintf(file, "_start:\n");
    for(size_t ip = 0; ip < parser_tokens.len; ++ip) {
        const parser_token_t *token = vec_get(&parser_tokens, ip);
        
        switch(token->operation) {
        case PARSER_PUSH: {
            char* command = malloc(sizeof(char)*20);
            sprintf(command, "push    %ld\n", token->data.u64_value);
            fprintf(file, "    %s", command);
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
            fprintf(file, "pop    rax\n");
            fprintf(file, "pop    rbx\n");
            fprintf(file, "cmp    rax, rbx\n");
            fprintf(file, "mov    rax, 0\n");
            fprintf(file, "mov    rbx, 1\n");
            fprintf(file, "cmove  rax, rbx\n");
            fprintf(file, "push   rax\n");
            break;
        }
        case PARSER_PRINT: {
            fprintf(file, "    pop     rdi\n");
            fprintf(file, "    call    print\n");
            break;
        }
        default:
            fprintf(stderr, "Unknow operation '%d'\n", token->operation);
            exit(ERR_UNKNOW_OPERATION);
        }
    }

    fprintf(file, "    mov     rdi, 0\n");
    fprintf(file, "    mov     rax, 0x3c\n");
    fprintf(file, "    syscall\n");

    fclose(file);

    system(NASM_CMD);
    system(LINKER_CMD);
}