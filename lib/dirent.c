#include <dirent.h>
#include <io.h>
#include <types.h>
#include <syscall.h>

RESULT(u64) opendir(const char* path) {
    u64 mode = O_RDONLY|O_DIRECTORY|O_CLOEXEC;
    return open(path, NULL, mode);
}

RESULT(u64) _getdents(u64 fd, Dirent* ent, u64 count) {
    i64 ret = (i64) syscall3(SYS_GETDENTS64, 
            (untyped)fd,
            ent,
            (untyped)count);
    if (ret < 0)
        return (RESULT(u64)){.success = FALSE, .errno = -ret};
    else
        return (RESULT(u64)){.success = TRUE, .value = ret};
}

RESULT(dirent) nextdir(u64 fd, DirectoryBuffer* buf) {
    bool new_entries = FALSE;
    while (TRUE) {
        if (buf->len == 0 || buf->len < buf->offset || new_entries) {
            RESULT(u64) ret;
            ret = _getdents(fd, buf->buffer, DIRENT_BUF_SIZE);
            if (!ret.success || ret.value == 0) {
                return (RESULT(dirent)){.success = FALSE, .errno = ret.errno};
            }
            buf->len = ret.value;
            buf->offset = 0;
            new_entries = FALSE;
        }

        Dirent* d;
        while (buf->offset < buf->len) {
            d = (Dirent*)(buf->buffer + buf->offset);
            buf->offset += d->len;
            if (d->ino != 0)
                return (RESULT(dirent)){.success = TRUE, .value = d};
        }
        new_entries = TRUE;
    }
}
