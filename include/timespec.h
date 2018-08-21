/* 
 * MIT License
 * 
 * Copyright (c) 2018 xiaofan <xfan1024@live.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __timespec_h__
#define __timespec_h__

#include <sys/time.h>
#include <time.h>

#define TIMESPEC_MS_PER_SEC 1000
#define TIMESPEC_US_PER_SEC 1000000
#define TIMESPEC_NS_PER_SEC 1000000000
#define TIMESPEC_US_PER_MS  1000
#define TIMESPEC_NS_PER_MS  1000000
#define TIMESPEC_NS_PER_US  1000

#define TIMESPEC_ROUND_DIV(a, b) (((a)+(b)/2)/(b))

static inline int time_t_cmp(time_t a, time_t b)
{
    time_t diff = a - b;
    if (diff < 0) {
        return -1;
    } else if (diff == 0) {
        return 0;
    } else {
        return 1;
    }
}

static inline int timespec_cmp(struct timespec a, struct timespec b)
{
    int res;
    res = time_t_cmp(a.tv_sec, b.tv_sec);
    if (res) {
        return res;
    }
    if (a.tv_nsec < b.tv_nsec) {
        return -1;
    } else if (a.tv_nsec == b.tv_nsec) {
        return 0;
    } else {
        return 1;
    }
}

static inline int timeval_cmp(struct timeval a, struct timeval b)
{
    int res;
    res = time_t_cmp(a.tv_sec, b.tv_sec);
    if (res) {
        return res;
    }
    if (a.tv_usec < b.tv_usec) {
        return -1;
    } else if (a.tv_usec == b.tv_usec) {
        return 0;
    } else {
        return 1;
    }
}

static inline int timespec_type(struct timespec ts)
{
    if (ts.tv_nsec < 0) {
        return -1;
    } else if (ts.tv_nsec < TIMESPEC_NS_PER_SEC) {
        return 0;
    } else {
        return 1;
    }
}

static inline int timeval_type(struct timeval tv)
{
    if (tv.tv_usec < 0) {
        return -1;
    } else if (tv.tv_usec < TIMESPEC_US_PER_SEC) {
        return 0;
    } else {
        return 1;
    }
}

static inline struct timespec timespec_normalize(struct timespec ts)
{
    switch(timespec_type(ts))
    {
    case -1:
        ts.tv_nsec += TIMESPEC_NS_PER_SEC;
        ts.tv_sec -= 1;
        break;
    case 1:
        ts.tv_nsec -= TIMESPEC_NS_PER_SEC;
        ts.tv_sec += 1;
        break;
    case 0:
        return ts;
    }
#if TIMESPEC_DEBUG
    assert(timespec_type(ts) == 0);
#endif
    return ts;
}

static inline struct timeval timeval_normalize(struct timeval tv)
{
    switch(timeval_type(tv))
    {
    case -1:
        tv.tv_usec += TIMESPEC_US_PER_SEC;
        tv.tv_sec -= 1;
        break;
    case 1:
        tv.tv_usec -= TIMESPEC_US_PER_SEC;
        tv.tv_sec += 1;
        break;
    case 0:
        return tv;
    }
#if TIMESPEC_DEBUG
    assert(timeval_type(tv) == 0);
#endif
    return tv;
}

static inline struct timespec timespec_from_timeval(struct timeval tv)
{
    struct timespec retval;
    retval.tv_nsec = tv.tv_usec * TIMESPEC_NS_PER_US;
    retval.tv_sec  = tv.tv_sec;
    return retval;
}

static inline struct timeval timespec_to_timeval(struct timespec ts)
{
    struct timeval retval;
    retval.tv_usec = TIMESPEC_ROUND_DIV(ts.tv_nsec, TIMESPEC_NS_PER_US);
    retval.tv_sec  = ts.tv_sec;
    return retval;
}

static inline struct timeval timeval_from_us(long us)
{
    struct timeval retval;
    retval.tv_usec = us % TIMESPEC_US_PER_SEC;
    retval.tv_sec  = us / TIMESPEC_US_PER_SEC;
    return timeval_normalize(retval);
}

static inline struct timeval timeval_from_ms(long ms)
{
    struct timeval retval;
    retval.tv_usec = (ms % TIMESPEC_MS_PER_SEC) * TIMESPEC_US_PER_MS;
    retval.tv_sec = ms / TIMESPEC_MS_PER_SEC;
    return timeval_normalize(retval);
}

static inline struct timeval timeval_from_sec(time_t sec)
{
    struct timeval retval;
    retval.tv_usec = 0;
    retval.tv_sec = sec;
    return retval;
}

static inline struct timeval timeval_neg(struct timeval tv)
{
    tv.tv_sec = -tv.tv_sec;
    tv.tv_usec = -tv.tv_usec;
    return timeval_normalize(tv);
}


static inline long timeval_to_us(struct timeval tv)
{
    return TIMESPEC_US_PER_SEC * tv.tv_sec + tv.tv_usec;
}

static inline long timeval_to_ms(struct timeval tv)
{
    return (TIMESPEC_MS_PER_SEC * tv.tv_sec) + TIMESPEC_ROUND_DIV(tv.tv_usec, TIMESPEC_US_PER_MS);
}

static inline time_t timeval_to_sec(struct timeval tv)
{
    return tv.tv_sec + TIMESPEC_ROUND_DIV(tv.tv_usec, TIMESPEC_US_PER_SEC);
}

static inline struct timespec timespec_sub_timespec_return_timespec(struct timespec a, struct timespec b)
{
    struct timespec diff;
    diff.tv_sec  = a.tv_sec - b.tv_sec;
    diff.tv_nsec = a.tv_nsec - b.tv_nsec;
    return timespec_normalize(diff);
}

static inline struct timespec timespec_add_timespec_return_timespec(struct timespec a, struct timespec b)
{
    struct timespec sum;
    sum.tv_sec  = a.tv_sec + b.tv_sec;
    sum.tv_nsec = a.tv_nsec + b.tv_nsec;
    return timespec_normalize(sum);
}

static inline struct timeval timeval_add_timeval(struct timeval a, struct timeval b)
{
    return  timespec_to_timeval(
                timespec_add_timespec_return_timespec(
                    timespec_from_timeval(a),
                    timespec_from_timeval(b)
                )
            );
}

static inline struct timeval timeval_sub_timeval(struct timeval a, struct timeval b)
{
    return  timespec_to_timeval(
                timespec_sub_timespec_return_timespec(
                    timespec_from_timeval(a),
                    timespec_from_timeval(b)
                )
            );
}


static inline struct timeval timespec_sub_timespec(struct timespec a, struct timespec b)
{
    return  timespec_to_timeval(
                timespec_sub_timespec_return_timespec(
                    a, 
                    b
                )
            );
}

static inline struct timespec timespec_sub_timeval(struct timespec ts, struct timeval tv)
{
    return  timespec_sub_timespec_return_timespec(
                ts, 
                timespec_from_timeval(tv)
            );
}

static inline struct timespec timespec_add_timeval(struct timespec ts, struct timeval tv)
{
    return  timespec_add_timespec_return_timespec(
                ts, 
                timespec_from_timeval(tv)
            );
}

static inline struct timespec timespec_current()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts;
}

static inline struct timespec timespec_since_boot()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts;
}

#endif

