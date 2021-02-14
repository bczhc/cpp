#include <stdio.h>
#include <threads.h>

//
// Created by zhc on 2/5/21.
//

static _Atomic int a = 0;

int f(void *p) {
    for (int i = 0; i < 10000; ++i) {
        ++a;
    }
    return 0;
}

int main() {
    thrd_t t[100];
    for (int i = 0; i < 100; ++i) {
        thrd_create(&(t[i]), f,NULL);
    }
    for (int i = 0; i < 100; ++i) {
        thrd_join(t[i], NULL);
    }
    printf("%i\n", a);
    return 0;
}