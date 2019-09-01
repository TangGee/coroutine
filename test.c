#include "coroutine.h"
#include <stdio.h>

void test3(const int *data) {
    printf("test3 param = %d\n", (*data));
    printf("enter %s", CURRENT_COROUTINE_NAME);
    coroutine_yield();
    printf("finish yield %s\n", CURRENT_COROUTINE_NAME);
}

void test2(const int *data) {
    printf("test2 param = %d\n", (*data));
    printf("enter %s", CURRENT_COROUTINE_NAME);
}

coroutine *co2;

void test1(int *data) {
    printf("enter %s", CURRENT_COROUTINE_NAME);
    co2 = create_coroutine((void (*)(void *)) test2, &data);
    coroutine_setname(co2, "co2");
    coroutine_resume(co2);
    printf("enter %s switch from %s\n", CURRENT_COROUTINE_NAME, coroutine_getname(co2));

    int i = 10;
    coroutine *co3 = create_coroutine((void (*)(void *)) test3, &i);
    coroutine_setname(co3, "co3");
    coroutine_resume(co3);
    printf("enter %s switch from %s\n", CURRENT_COROUTINE_NAME, coroutine_getname(co3));
    coroutine_resume(co3);
    printf("enter %s switch from %s\n", CURRENT_COROUTINE_NAME, coroutine_getname(co3));
}

int main() {
    int data = 1;
    coroutine *co1 = create_coroutine((void (*)(void *)) test1, &data);
    coroutine_setname(co1, "co1");
    coroutine_resume(co1);
    printf("hahha\n");
    return 0;
}