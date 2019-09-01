//
// Created by ermei on 2019/9/1.
//

#ifndef COROUTINE_COROUTINE_H
#define COROUTINE_COROUTINE_H

#define CURRENT_COROUTINE_NAME coroutine_getname(current_coroutine())

typedef struct coroutine coroutine;

typedef void coroutine_entry(void *opaque);

coroutine *create_coroutine(coroutine_entry *entry, void *data);

coroutine *current_coroutine();

void coroutine_yield();

void coroutine_resume(coroutine *co);

void delete_coroutine(coroutine *co);

void coroutine_setname(coroutine *co, const char *name);

const char *coroutine_getname(coroutine *co);

#endif //COROUTINE_COROUTINE_H
