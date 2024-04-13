.global syscall0, syscall1, syscall2, syscall3, syscall4, syscall5
.section .text

# essentially translate between x64 System V userspace and x64 System V kernel space calling conventions
# user space   : %rdi, %rsi, %rdx, %rcx, %r8, %r9
# kernel space : %rdi, %rsi, %rdx, %r10, %r8, %r9
# Those seem pretty similar to each other until you realize that the syscall number, which with any sane design would be the first argument is passed in %rax
# so we effectively need to shift the registers:

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
