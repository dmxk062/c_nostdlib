# vim: commentstring=#\ %s

.global _start, syscall_raw


.section .text

_start:
    xor     %rbp,   %rbp
    # move argc into the first argument
    pop     %rdi
    # move argv into the second arg
    mov     %rsp,   %rsi

    # align stack
    and     $-16,   %rsp
    call    main

    # exit from main with the code returned by main
    mov     %rax,   %rdi
    mov     $60,    %rax
    syscall 

    ret

syscall_raw:
    # syscall number
    mov     %rdi,   %rax

    # arguments
    mov     %rsi,   %rdi
    mov     %rdx,   %rsi
    mov     %rcx,   %rdx
    mov     %r8,    %r10
    mov     %r9,    %r8

    syscall
    ret
