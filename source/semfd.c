#include <semfd.h>
#include <die.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <sys/types.h>

int xsemfd(int cnt)
{
    int semfd = eventfd(cnt, EFD_SEMAPHORE);
    if (semfd < 0) {
        die_perror("eventfd, res=%d", semfd);
    }
    return semfd;
}

void xsemfd_post(int semfd)
{
    int res;
    uint64_t one = 1;
    res = write(semfd, &one, sizeof(one));
    if (res == sizeof(one)) {
        return;
    }
    die_perror("write, res=%d", res);
}


int xsemfd_wait(int semfd, struct timeval * timeout)
{
    int res;
    uint64_t one;
    if (timeout) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(semfd, &fds);
        res = select(semfd+1, &fds, NULL, NULL, timeout);
        if (res == 0) {
            return 0;
        } else if (res != 1) {
            die_perror("select, res=%d", res);
        }
    }
    res = read(semfd, &one, sizeof(one));
    if (res != sizeof(one)) {
        die_perror("read, res=%d", res);
    }
    if (one != 1) {
        die_perror("read, value=%llu\n", one);
    }
    return 1;
}

int xsemfd_trywait(int semfd)
{
    struct timeval tv = {0, 0};
    return xsemfd_wait(semfd, &tv);
}
