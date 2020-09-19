#include <pthread.h>

namespace bczhc {
namespace thread {
class Runnale {
public:
    virtual void run() = 0;
};

template <typename ArgType> class Consumer {
public:
    virtual void accept(ArgType &arg) = 0;
};

void *call(void *arg) {
    ((Runnale *)arg)->run();
    return nullptr;
}

template <typename T> struct Bean {
    Consumer<T> *c;
    void *arg;
};

template <typename ArgType> class ConsumerCall {
public:
    static void consume(ArgType &arg) {}

    static void *call(void *arg) {
        Bean<ArgType> &b = *(Bean<ArgType> *)arg;
        ((Consumer<ArgType> *)b.c)->accept(*(ArgType *)b.arg);
        delete &b;
        return nullptr;
    }
};

class Thread {
private:
    pthread_t t;

public:
    Thread(Runnale &runnable) { pthread_create(&t, nullptr, call, &runnable); }

    template <typename ArgType>
    explicit Thread(Consumer<ArgType> &consumer, ArgType &arg) {
        Bean<ArgType> *b = new Bean<ArgType>;
        b->arg = &arg, b->c = &consumer;
        pthread_create(&t, nullptr, ConsumerCall<ArgType>::call, b);
    }

    void join() { pthread_join(t, nullptr); }
};
}; // namespace thread
}; // namespace bczhc
