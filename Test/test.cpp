#include <iostream>

using namespace std;

class Base {
public:
    virtual void f() { cout << "Base" << endl; };
};

int main() {
    class A : public Base {
    public:
        void f() override { cout << "A" << endl; }
    };
    class B : public Base {
    public:
        void f() override { cout << "B" << endl; }
    };
    Base *a = new A;
    Base *b = new B;
    a->f(), b->f();
    return 0;
}
