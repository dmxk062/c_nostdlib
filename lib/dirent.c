#include <dirent.h>
#include <io.h>
#include <types.h>
#include <syscall.h>

Result(u64) opendir(const char* path) {
    u64 mode = O_RDONLY|O_DIRECTORY|O_CLOEXEC;
    return open(path, NULL, mode);
}

Result(u64) _getdents(u64 fd, Dirent* ent, u64 count) {
    i64 ret = (i64) syscall3(SYS_GETDENTS64, 
            (untyped)fd,
            ent,
            (untyped)count);
    if (ret < 0)
        return Err(u64, -ret);
    else
        return Ok(u64, ret);
}

PResult(Dirent) nextdir(u64 fd, DirectoryBuffer* buf) {
    bool new_entries = false;
    while (true) {
        if (buf->len == 0 || buf->len < buf->offset || new_entries) {
            Result(u64) ret;
            ret = _getdents(fd, (Dirent*)buf->buffer, DIRENT_BUF_SIZE);
            if (!ret.ok || ret.value == 0) {
                return PErr(Dirent, ret.errno);
            }
            buf->len = ret.value;
            buf->offset = 0;
            new_entries = false;
        }

        Dirent* d;
        while (buf->offset < buf->len) {
            d = (Dirent*)(buf->buffer + buf->offset);
            buf->offset += d->len;
            if (d->ino != 0)
                return POk(Dirent, d);
        }
        new_entries = true;
    }
}
