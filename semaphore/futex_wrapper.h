#ifndef FUTEX_WRAPPER_H
#define FUTEX_WRAPPER_H


#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>


static int futex(int *uaddr, int futex_op, int val) {
    return syscall(SYS_futex, uaddr, futex_op, val, NULL, NULL, 0);
}

static void futex_wait(atomic_int *addr, int val) {
    futex((int *)addr, FUTEX_WAIT, val);
}

static void futex_wake(atomic_int *addr, int count) {
    futex((int *)addr, FUTEX_WAKE, count);
}

#endif