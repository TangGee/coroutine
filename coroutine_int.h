//
// Created by ermei on 2019/9/1.
//

#ifndef COROUTINE_COROUTINE_INT_H
#define COROUTINE_COROUTINE_INT_H

#include <sys/types.h>
#include <setjmp.h>
#include "coroutine.h"

#define MAX_COROUTINE_NAME_LENGTH 20

union cc_arg {
    void *p;
    int i[2];
};

typedef enum {
    COROUTINE_YIELD = 1,
    COROUTINE_TERMINATE = 2,
    COROUTINE_ENTER = 3,
} coroutine_action;

struct coroutine {
    size_t stack_size;
    void *stack_sp;
    void *data;
    coroutine_entry *entry;
    sigjmp_buf context_env;
    sigjmp_buf init_env;
    struct coroutine *caller;
    char name[MAX_COROUTINE_NAME_LENGTH];
};

void coroutine_trampoline(int arg1, int arg2);

int coroutine_switch(coroutine *from, coroutine *to, coroutine_action action);

#endif //COROUTINE_COROUTINE_INT_H
