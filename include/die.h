#ifndef __c_utils_die__
#define __c_utils_die__

#define die_perror(fmt, ...) __die_perror_with_source_info(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

void __die_perror_with_source_info(const char * file, const char* function, int line, const char* fmt, ...);

#endif
