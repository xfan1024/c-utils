/*
 * exception mechanism based on setjmp/longjmp
 */

#ifndef __exception_mechanism_h__
#define __exception_mechanism_h__

#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <stdlib.h>

struct exception_handler {
    jmp_buf jb;
};

#define e_try(eh) { \
    struct exception_handler * __local_eh = (eh); \
    volatile int __local_err; \
    __local_err = setjmp(__local_eh->jb); \
    if (__local_err == 0) {

#define e_throw(eh, err)    do { int __throw_err = (err); assert(__throw_err); longjmp((eh->jb), __throw_err); } while (0)
#define e_catch(err)        } else if (__local_err == (err)) {
#define e_catch_all(err)    } else if (1) { int err = __local_err;
#define e_finish_throw(eh)  } else { e_throw((eh), __local_err); } }
#define e_finish            } else { fprintf(stderr, "uncaught error: %d\n", __local_err); abort(); } }

#endif
