//
// Created by ermei on 2019/9/1.
//

#ifndef COROUTINE_STACK_H
#define COROUTINE_STACK_H

#include <sys/types.h>

void *coroutine_alloc_stack(size_t *size);

void coroutine_free_stack(void *stack, size_t sz);

#endif //COROUTINE_STACK_H
