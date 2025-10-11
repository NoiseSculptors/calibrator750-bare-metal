
#include "syscall.h"

// route you own _write_char() to ex. USART1
void _write_char(char c)
{
    (void)c;
}

int _write(int fd, const void *buf, size_t count)
{
    (void)fd;
    const char *cbuf = buf;
    for (size_t i = 0; i < count; i++) {
        _write_char(cbuf[i]);
    }
    return (int)count;
}

int _read(int fd, void *buf, size_t count)
{
    (void)fd; (void)buf; (void)count;
    errno = ENOSYS;
    return -1;
}

int _close(int fd)
{
    (void)fd;
    errno = ENOSYS;
    return -1;
}

off_t _lseek(int fd, off_t offset, int whence)
{
    (void)fd; (void)offset; (void)whence;
    errno = ENOSYS;
    return -1;
}

int _fstat(int fd, struct stat *st)
{
    (void)fd;
    st->st_mode = S_IFCHR; // pretend it's a character device
    return 0;
}

int _isatty(int fd)
{
    (void)fd;
    return 1;
}

// Optional: only if you use malloc()
extern char _end;   // provided by linker
static char *heap_end;

void *_sbrk(ptrdiff_t incr)
{
    if (!heap_end)
        heap_end = &_end;
    char *prev = heap_end;
    heap_end += incr;
    return prev;
}

