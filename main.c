#include <stdio.h>
#include <ucontext.h>
#include <setjmp.h>
#include <stdlib.h>
#include <zconf.h>
#include <sys/mman.h>

#ifndef ROUND_UP
#define ROUND_UP(n, d) (((n) + (d) - 1) & -(0 ? (n) : (d)))
#endif

union cc_arg {
    void *p;
    int i[2];
};
void coroutine_trampoline(int arg1, int arg2) {
    union cc_arg arg;
    sigjmp_buf new_env;

    arg.i[0] = arg1;
    arg.i[1] = arg2;

    sigjmp_buf *env = arg.p;

    if (!sigsetjmp(new_env, 0)) {
        siglongjmp(env, 1);
    }

    // only sigsetjmp return 1 can execute after code
    printf("hahh\n");
}

void* alloc_stack(size_t *size) {
    void *ptr, *guardpage;
    int pagesize = getpagesize();
#ifdef _SC_THREAD_STACK_MIN
    long min_stack_size = sysconf(_SC_THREAD_STACK_MIN);
    (*size) = (*size) > min_stack_size ? (*size) : min_stack_size;
#endif
    (*size) = ROUND_UP(*size, pagesize);
    //allocte one extra page for the guard page
    (*size) += pagesize;
    ptr = mmap(NULL, (*size), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
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

int main() {
    ucontext_t old_uc, uc;
    sigjmp_buf env;
    union cc_arg arg = {0};

    size_t stack_size = 1 << 20;

    if (getcontext(&uc) == -1) {
        abort();
    }
    printf("set context\n");


    uc.uc_link = &old_uc;
    uc.uc_stack.ss_sp = alloc_stack(&stack_size);
    uc.uc_stack.ss_size = stack_size;
    uc.uc_stack.ss_flags = 0;

    arg.p = &env;
    makecontext(&uc, (void (*)(void))coroutine_trampoline, 2, arg.i[0], arg.i[1]);
    printf("make context\n");

    if (!sigsetjmp(env, 0)) {
        printf("before swapcontext\n");
        swapcontext(&old_uc, &uc);
        printf("end swapcontext\n");

    }
    
}