//
// Created by zhc on 2/5/21.
//

int main() {
    struct A {
        int a, b, c;
        char s[5];
        struct A *p;
    };


    struct A a = {
            1, 2, 3, {1, 2, 3, 4, 5}, (void *) 0
    };

    return a.s[0];
}