#include <stat.h>
#include <syscall.h>
#include <types.h>

errno_t stat(char* path, struct stat* statbuf) {
    return (errno_t)-(i64)
    syscall2(SYS_STAT, 
            path, 
            statbuf);
}
errno_t lstat(char* path, struct stat* statbuf) {
    return (errno_t)-(i64)
    syscall2(SYS_STAT, 
            path, 
            statbuf);
}

errno_t fstat(u64 fd, struct stat* statbuf) {
    return (errno_t)-(i64)
    syscall2(SYS_FSTAT, 
            (void*)fd, 
            statbuf);
}
