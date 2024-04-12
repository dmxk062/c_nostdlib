# vim: commentstring=#\ %s

.global _start 
.extern main



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

    # ret

