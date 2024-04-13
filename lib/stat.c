#include "stat.h"
#include "syscall.h"

i64 stat(char* path, struct stat* statbuf) {
    return (i64)
    syscall2(SYS_STAT, 
            path, 
            statbuf);
}
i64 lstat(char* path, struct stat* statbuf) {
    return (i64)
    syscall2(SYS_STAT, 
            path, 
            statbuf);
}

i64 fstat(u64 fd, struct stat* statbuf) {
    return (i64)
    syscall2(SYS_FSTAT, 
            (void*)fd, 
            statbuf);
}
