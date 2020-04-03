//
// Created by root on 2020/3/27.
//

class A {
    virtual double f(double x) = 0;
};

class B : A {
public:
    double f(double x) override {
        return x + 1;
    }
};

int main() {
    int a = 1;
    class C : A {
        double f(double x) override {
            return a;
        }
    };
    return 0;
}