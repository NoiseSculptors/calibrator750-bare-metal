#ifndef SYSCALL_H
#define SYSCALL_H

#include <stddef.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// route you own _write_char() to ex. USART1
void _write_char(char c);

int _write(int fd, const void *buf, size_t count);
int _read(int fd, void *buf, size_t count);
int _close(int fd);
off_t _lseek(int fd, off_t offset, int whence);
int _fstat(int fd, struct stat *st);
int _isatty(int fd);
void *_sbrk(ptrdiff_t incr);

#endif
