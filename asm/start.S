.global _start

# this won't be linked in the same compilation unit as main
.extern __no_libc_start

.section .data
    # store a pointer to the first element of environ
    environ: .quad 0

.section .text

# before calling main, _start has to setup the arguments and the environment
# the x64 System V calling convention requires the first three arguments to a userspace function to be in:
# %rdi, %rsi and %rdx

# therefore for main(int argc, char* argv[], char* environ[]) to work, 
# we need to move the addresses/values from the stack into these registers

# upon start, a program's stack looks something like this:
# argc                  (32 bits)
# argv[0]               
# argv[1]               
# ...                   (64 bits each)
# argv[argc - 1]        
# NULL                  (64 bits)
# environ[0]            
# environ[1] 
# ...                   (64 bits each)
# environ[n - 1]
# NULL                  (64 bits)




_start:
    xor     %rbp,   %rbp

    # move argc off the stack into the first argument
    pop     %rdi

    # move the address of the first element of argv into the second argument
    mov     %rsp,   %rsi

    # align stack to 16 bits since we popped 32 bit argc
    and $0xFFFFFFFFFFFFFFF0,%rsp

    # call the libc entrypoint
    call    __no_libc_start

    # exit from the program with the code returned by main
    mov     %rax,   %rdi
    mov     $60,    %rax

    syscall 

    # we shoudn't need to return here, if we ever reach this, we will segfault
    ret
