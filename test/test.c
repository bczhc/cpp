//
// Created by zhc on 2/5/21.
//

#include <stdio.h>
#include <pthread.h>

void* f() {
    printf("%s\n", "hello, world");
    return NULL;
}

int main() {
    pthread_t t;
    pthread_create(&t, NULL, f, NULL);
    pthread_join(t, NULL);
    return 0;
}