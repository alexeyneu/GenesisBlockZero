section .data
msg resb 32
db 10
hex_nums db '0123456789ABCDEF'
xb dq 0FF0FEFCE5A07E1CBh , 0EC0ACFBD7107A1BBh
xx resq 2
length dw 16
a1 equ 0
a2 equ 0
a3 equ 1
a4 equ 0
a equ a1 | (a2 << 2) | (a3 << 4) | (a4 << 6) 
section .text
global main

main:
    movdqu xmm1, [xb]
    pshufd xmm0, xmm1, a
    movdqu [xx], xmm0

    mov rcx, 0
    mov rax, 0
    mov rbx, 0
sw:
    mov ah, [rcx + xx]
    mov bl, ah
    shr bl, 0x04
    mov al, [rbx + hex_nums]
    mov [rcx*2 + msg], al
    and ah, 0x0F
    mov bl, ah
    mov ah, [rbx + hex_nums]
    mov [rcx*2 + msg + 1], ah
    inc cx
    cmp cx, [length]
    jl  sw

    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 33 ;32+
    syscall

    mov rax, 60
    mov rdi, 0
    syscall


;nasm -f elf64 x.asm -o t.o
;gcc -no-pie t.o -o t
