#ifndef __c_utils_semfd_h__
#define __c_utils_semfd_h__

#include <sys/time.h>

int  xsemfd(int cnt);
void xsemfd_post(int semfd);
int  xsemfd_wait(int semfd, struct timeval * timeout);
int  xsemfd_trywait(int semfd);

#endif
