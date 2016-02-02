#ifndef  ATOMIC_H
#define  ATOMIC_H

#include <stdint.h>

// return old value
static inline int atomic_add(volatile void* count, int add) {
    __asm__ __volatile__(
        "lock xadd %0, (%1);"
        : "=a"(add)
        : "r"(count), "a"(add)
        : "memory");
    return add;
}

// return old value
static inline int atomic_swap(volatile void* lockword, int value) {
    __asm__ __volatile__(
        "lock xchg %0, (%1);"
        : "=a"(value)
        : "r"(lockword), "a"(value)
        : "memory");
    return value;
}

// return old value
static inline int atomic_comp_swap(volatile void* lockword,
                                   int exchange,
                                   int comperand) {
    __asm__ __volatile__(
        "lock cmpxchg %1, (%2);"
        : "=a"(comperand)
        : "d"(exchange), "r"(lockword), "a"(comperand));
    return comperand;
}

#define atomic_inc(x)               (atomic_add(&(x), 1) + 1)
#define atomic_dec(x)               (atomic_add(&(x), -1) - 1)
#define atomic_get_value(x)         (atomic_comp_swap(&(x), 0, 0))
#define atomic_set_value(x, v)      (atomic_swap(&(x), (v)))


#endif  

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */