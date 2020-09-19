#include <iostream>

using namespace std;

int i = 0;

pthread_mutex_t lock;

void *f1(void *arg) {
    for (int j = 0; j < 10000000; ++j) {
        pthread_mutex_lock(&lock);
        ++i;
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}

void *f2(void *arg) {
    for (int j = 0; j < 10000000; ++j) {
        pthread_mutex_lock(&lock);
        ++i;
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, f1, nullptr);
    pthread_create(&t2, nullptr, f2, nullptr);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    cout << i << endl;
    return 0;
}
