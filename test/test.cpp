#include "../zhc.h"
#include <cstdio>

int main() {
    Queue<int> q;
    q.enqueue(0);
    printf("%i\n", q.dequeue());
    q.enqueue(1);
    printf("%i\n", q.dequeue());
    return 0;
}
