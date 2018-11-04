/*
 * simple exception support environment
 */

#ifndef __ese_h__
#define __ese_h__

#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define ESE_NESTED_MAX 4

#if 0
#define ese_log(fmt, args...) do { fprintf(stderr, "%s:%d ", __func__, __LINE__); fprintf(stderr, fmt, ##args); } while (0)
#else
#define ese_log(fmt, args...) do { } while (0)
#endif

struct ese {
    jmp_buf         jp[ESE_NESTED_MAX];
    volatile int    top;
};

#define ese_init(_ese) memset((_ese), 0, sizeof(struct ese))

#define ese_try(_ese, err) \
{ \
    struct ese * __local_ese; \
    int __local_ese_err, err; \
    __local_ese = (_ese); \
    ese_log("ese_try: top=%d\n", __local_ese->top); \
    assert(0 <= __local_ese->top && __local_ese->top < ESE_NESTED_MAX); \
    __local_ese_err = err = setjmp(__local_ese->jp[__local_ese->top++]); \
    if (__local_ese_err == 0)

#define ese_catch_if(cond)          else if ( (cond) )
#define ese_catch(e)                ese_catch_if((e) == __local_ese_err)
#define ese_catch_all               else if (1)

#define ese_early_finish do \
{ \
    if (!__local_ese_err) { \
        __local_ese->top--; \
    } \
    assert(0 <= __local_ese->top && __local_ese->top < ESE_NESTED_MAX); \
} while (0)

#define ese_throw_error(_ese, err) do \
{ \
    ese_log("ese_throw_error: top=%d, err=%d\n", (_ese)->top, err); \
    assert(err < 0); \
    if (0 == (_ese)->top) { \
        fprintf(stderr, "ese_throw_error: unhandle exception: %d\n", err); \
        abort(); \
    } \
    assert(0 < (_ese)->top && (_ese)->top <= ESE_NESTED_MAX); \
    longjmp((_ese)->jp[--(_ese)->top], err); \
} while (0)

#define ese_finish \
    else { ese_throw_error(__local_ese, __local_ese_err); } \
    ese_early_finish; \
    ese_log("ese_finish: top=%d, __local_ese_err=%d\n", __local_ese->top, __local_ese_err); \
    assert(0 <= __local_ese->top && __local_ese->top < ESE_NESTED_MAX); \
}

#define ese_early_return(val) do \
{ \
    typeof(val) __early_retval = (val); \
    ese_early_finish; \
    return __early_retval; \
} while (0)


#define ese_check_top(_ese, expect) assert((_ese)->top == expect)

#endif
