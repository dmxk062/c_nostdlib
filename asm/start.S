# vim: commentstring=#\ %s

.global _start, environ
.extern main

.section .data
    environ: .quad 0

.section .text

# before calling main, _start has to setup the arguments and the environment
# the x64 SystemV calling convention requires the first three arguments to a userspace function to be in:
# %rdi, %rsi and %rdx
# so that main(int argc, char* argv[], char* environ[]) can work, 
# we need to move the addresses/values from the stack into these registers
_start:
    xor     %rbp,   %rbp
    # move argc into the first argument
    pop     %rdi
    # move argv into the second arg
    mov     %rsp,   %rsi

    # calculate the position of the environment
    # use argc as an offset
    # this makes it possible to do:
    # int main(int argc, char* argv[], char* environ[]);
    imul    $8,%rdi,%rdx
    add     %rsp,   %rdx
    add     $8,     %rdx

    # extern char** environ, the other common way to access it
    lea     environ(%rip),%r8
    mov     %rdx,   (%r8)

    # align stack
    and     $-16,   %rsp
    # call the main function
    call    main

    # exit from main with the code returned by main
    mov     %rax,   %rdi
    mov     $60,    %rax
    syscall 

    # ret

