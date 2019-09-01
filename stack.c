//
// Created by ermei on 2019/9/1.
//
#include "stack.h"
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ROUND_UP
#define ROUND_UP(n, d) (((n) + (d) - 1) & -(0 ? (n) : (d)))
#endif

void *coroutine_alloc_stack(size_t *size) {
    void *ptr, *guardpage;
    int pagesize = getpagesize();
#ifdef _SC_THREAD_STACK_MIN
    long min_stack_size = sysconf(_SC_THREAD_STACK_MIN);
    (*size) = (*size) > min_stack_size ? (*size) : min_stack_size;
#endif
    (*size) = ROUND_UP(*size, pagesize);
    //allocte one extra page for the guard page
    (*size) += pagesize;
    ptr = mmap(NULL, (*size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("failed to allocate memory for stack");
        abort();
    }
#if defined(HOST_IA64)
    /* separate register stack */
    guardpage = ptr + (((*sz - pagesz) / 2) & ~pagesz);
#elif defined(HOST_HPPA)
    /* stack grows up */
    guardpage = ptr + *sz - pagesz;
#else
    /* stack grows down */
    guardpage = ptr;
#endif
    if (mprotect(guardpage, pagesize, PROT_NONE)) {
        perror("failed to set up stack guard page");
        abort();
    }

    return ptr;
}

void coroutine_free_stack(void *stack, size_t sz) {
    munmap(stack, sz);
}

