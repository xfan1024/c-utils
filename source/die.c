#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void die_vperror(const char* fmt, va_list arg)
{
    if (fmt) {
        fprintf(stderr, "program message: \n");
        fprintf(stderr, "    ");
        vfprintf(stderr, fmt, arg);
        fprintf(stderr, "\n");
    }
    if (errno) {
        fprintf(stderr, "errno detect: \n");
        fprintf(stderr, "    %s(%d)\n", strerror(errno), errno);
    }
    abort();
}

void __die_perror_with_source_info(const char * file, const char* function, int line, const char* fmt, ...)
{
    va_list arg;
    fprintf(stderr, "program crash at: \n");
    fprintf(stderr, "    file: %s\n", file);
    fprintf(stderr, "    func: %s\n", function);
    fprintf(stderr, "    line: %d\n", line);
    
    va_start (arg, fmt);
    die_vperror(fmt, arg);
    va_end(arg);
}
