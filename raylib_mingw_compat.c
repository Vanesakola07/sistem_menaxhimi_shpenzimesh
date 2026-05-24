#ifdef _WIN32
#include <sys/stat.h>

/*
 * Some prebuilt raylib MinGW archives reference the POSIX-style symbol
 * stat64i32, while UCRT MinGW exposes the same function as _stat64i32.
 */
int stat64i32(const char *name, struct _stat64i32 *statbuf) {
    return _stat64i32(name, statbuf);
}
#endif
