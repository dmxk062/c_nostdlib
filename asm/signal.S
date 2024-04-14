.global sigaction_trampoline

#
# Thanks to linux having a weird api for signals, we need to do this
# The SYS_rt_sigaction syscall needs a restorer label inside the struct passed to it
# This restorer cannot call C-style function and cannot be one itself
# So we do this
# *If* you ever need your own restorer, use `asm("mov $15, %rax\nsyscall")` for amd64
# setsigaction(the wrapper around sigaction) does this for you
#

.section .text

sigaction_trampoline:
    mov     $15,    %rax
    syscall
