segment .data
    b db "B() is executing.",10
    c db "ASCII of Input : "
segment .text 
    global B
    extern C
B:  
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32
    mov     [rbp - 24], rdi
    mov     rdi, rax
    lea     rax, [rbp -24]
    mov     [rbp - 8], rax
    mov     rax, [rbp - 8]
    mov     rdi, rax
    mov     rax, 1
    mov     rdi, 1
    mov     rsi, b
    mov     rdx, 18
    syscall
    mov     rax, 1
    mov     rdi, 1
    mov     rsi, c
    mov     rdx, 18
    syscall
    mov     rax, 1
    mov     rdi, 1
    mov     rsi, [rbp - 8]
    mov     rdx, 8
    syscall
    pop     rax
    lea     rsi, [C]
    push    rsi
    ret