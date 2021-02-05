//
// Created by zhc on 2/5/21.
//

#include <stdio.h>
#include "api.h"
#include <malloc.h>

int main() {
    char *s = NULL;
    toString(&s, 54321);
    printf("%s\n", s);
    free(s);
    return 0;
}