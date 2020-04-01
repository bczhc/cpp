//
// Created by root on 2020/3/31.
//

#include "./ComplexValue.h"
#include "./ComplexDefinite.h"
#include <iostream>
#include <thread>
#include "./Epicycle.h"
#include "./zhc.h"

using namespace std;

class FourierSeries {
private:
    int32_t omega{};
    int32_t T{};
    OneVariableComplexFunction_t f{};
    int32_t epicyclesCount{};

    void tempFunction(ComplexValue &cv, _Float64 t, _Float64 n) {
        f(cv, t);
        cv.selfMultiply(cosf64(n * t * omega), sinf64(-n * t * omega));
    }

public:
    explicit FourierSeries(OneVariableComplexFunction_t pFunction, int32_t period, int32_t _epicyclesCount) {
        f = pFunction;
        T = period;
        omega = M_PIf64 * 2 / period;
        epicyclesCount = _epicyclesCount;
    }

    void calc(List<Epicycle> &list) {
        int32_t a = -epicyclesCount / 2;
        int32_t t = a + epicyclesCount;
        ComplexValue c1;
        ComplexDefinite d{};
        for (int32_t n = a; n < t; ++n) {
            tempFunction(c1, t, n);
            ComplexValue df = d.getDefiniteIntegralByTrapezium(0, T, f);
            Epicycle e(n, df.selfDivide(T, 0));
            list.add(e);
        }
    }
};

void f(ComplexValue &cv, _Float64 t) {
    cv.setValue(t * t, sqrtf64(t));
}

void tF() {
    FourierSeries fourierSeries(f, 10, 1000);
    List<Epicycle> list{};
    fourierSeries.calc(list);
}

int main() {
    thread *t[4];
    for (auto &i : t) {
        i = new thread(tF);
    }
    for (auto &i : t) {
        i->join();
    }
    for (auto &i : t) {
        delete i;
    }
    cout << "done" << endl;
    return 0;
}