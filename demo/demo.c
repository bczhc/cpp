//
// Created by zhc on 2/5/21.
//

#include <stdio.h>

int main() {
    int a[] = {1, 2, 3};
    char *p = (char*) a;
    printf("%p\n", p);
    printf("%p\n", p + sizeof(int) * 1);
    return 0;
}