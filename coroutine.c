//
// Created by ermei on 2019/9/1.
//

#include <sys/types.h>
#include <setjmp.h>
#include "coroutine_int.h"
#include <ucontext.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "coroutine.h"

static __thread coroutine *current;
static __thread coroutine leader;

coroutine *create_coroutine(coroutine_entry *entry, void *data) {
    ucontext_t old_uc, uc;
    coroutine *co;
    union cc_arg arg = {0};

    size_t stack_size = 1 << 20;

    if (getcontext(&uc) == -1) {
        abort();
    }

    uc.uc_link = &old_uc;
    uc.uc_stack.ss_sp = coroutine_alloc_stack(&stack_size);
    uc.uc_stack.ss_size = stack_size;
    uc.uc_stack.ss_flags = 0;

    co = malloc(sizeof(coroutine));
    if (!co) {
        abort();
    }
    co->stack_size = uc.uc_stack.ss_size;
    co->stack_sp = uc.uc_stack.ss_sp;
    co->caller = NULL;

    arg.p = co;
    makecontext(&uc, (void (*)(void)) coroutine_trampoline, 2, arg.i[0], arg.i[1]);

    if (!sigsetjmp(co->init_env, 0)) {
        swapcontext(&old_uc, &uc);
    }

    co->entry = entry;
    co->data = data;
    return co;
}

void coroutine_trampoline(int arg1, int arg2) {
    union cc_arg arg;

    arg.i[0] = arg1;
    arg.i[1] = arg2;

    coroutine *co = arg.p;

    if (!sigsetjmp(co->context_env, 0)) {
        siglongjmp(co->init_env, 1);
    }

    while (1) {
        co->entry(co->data);
        coroutine_switch(co, co->caller, COROUTINE_TERMINATE);
    }
}

int coroutine_switch(coroutine *from, coroutine *to, coroutine_action action) {

    current = to;
    int ret = sigsetjmp(from->context_env, 0);
    if (!ret) {
        siglongjmp(to->context_env, action);
    }
    return ret;
}

coroutine *current_coroutine() {
    if (!current) {
        return &leader;
    }
    return current;
}

void coroutine_yield() {
    coroutine *from = current_coroutine();
    coroutine *to = from->caller;

    if (!to) {
        abort();
    }
    from->caller = NULL;
    coroutine_switch(from, to, COROUTINE_YIELD);
}

void coroutine_resume(coroutine *co) {
    if (co->caller) {
        abort();
    }

    coroutine *from = current_coroutine();
    co->caller = from;

    int ret = coroutine_switch(from, co, COROUTINE_ENTER);

    switch (ret) {
        case COROUTINE_YIELD:
            break;
        case COROUTINE_TERMINATE:
            delete_coroutine(co);
            break;
        default:
            abort();
    }
}

void delete_coroutine(coroutine *co) {
    coroutine_free_stack(co->stack_sp, co->stack_size);
    free(co);
}

void coroutine_setname(coroutine *co, const char *name) {
    snprintf(co->name, MAX_COROUTINE_NAME_LENGTH, "%s", name);
}

const char *coroutine_getname(coroutine *co) {
    return co->name;
}