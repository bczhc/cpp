#include "../concurrent.hpp"
#include "../array_list.hpp"
#include <ctime>
#include <unistd.h>

using namespace bczhc;
using Condetion = MutexLock::Condition;

class Message {
public:
    int a;
};

class BlockingQueue {
private:
    ArrayList<Message> queue;
    MutexLock lock;

public:
    void put(Message message) {
        lock.lock();
        queue.insert(message);
        lock.notify();
        lock.unlock();
    }

    Message take() {
        lock.lock();
        while (queue.isEmpty()) {
            lock.wait();
        }
        Message msg = queue.remove(0);
        lock.unlock();
        return msg;
    }
};

BlockingQueue q;

int main() {
    class R : public Runnable {
    public:
        void run() override {
            for (;;) {
                Message m = q.take();
                printf("%i\n", m.a);
            }
        }
    } r;
    Thread t(&r);
    for (int i = 0; i < 10; ++i) {
        Message msg{};
        msg.a = i;
        q.put(msg);
    }
    t.join();
    return 0;
}
