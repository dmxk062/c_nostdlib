.global syscall0, syscall1, syscall2, syscall3, syscall4, syscall5
.section .text


syscall0:
    mov     %rdi,   %rax

    syscall
    ret

syscall1:
    mov     %rdi,   %rax

    mov     %rsi,   %rdi

    syscall
    ret

syscall2:
    mov     %rdi,   %rax

    mov     %rsi,   %rdi
    mov     %rdx,   %rsi

    syscall
    ret

syscall3:
    mov     %rdi,   %rax

    mov     %rsi,   %rdi
    mov     %rdx,   %rsi
    mov     %rcx,   %rdx

    syscall
    ret

syscall4:
    mov     %rdi,   %rax

    mov     %rsi,   %rdi
    mov     %rdx,   %rsi
    mov     %rcx,   %rdx
    mov     %r8,    %r10

    syscall
    ret

syscall5:
    mov     %rdi,   %rax

    mov     %rsi,   %rdi
    mov     %rdx,   %rsi
    mov     %rcx,   %rdx
    mov     %r8,    %r10
    mov     %r9,    %r8

    syscall
    ret
