//
// Created by root on 2020/3/31.
//

#include "./FourierSeries.h"

void tF() {
    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
            dest.re = t + 1;
            dest.im = dest.re * t - sqrt(dest.re);
        }
    };
    F f;
    FourierSeries fourierSeries(f, 40, 10);
    List<Epicycle> list{};
    fourierSeries.calc(list);
    list.release();
}

int main() {
    /*thread *t[4];
    for (auto &i : t) {
        i = new thread(tF);
    }
    for (auto &i : t) {
        i->join();
    }
    for (auto &i : t) {
        delete i;
    }*/
    tF();
    cout << "done" << endl;
    return 0;
}

void FourierSeries::tempFunction(ComplexValue &cv, double t, double n) {
    f.x(cv, t);
    cv.selfMultiply(cos(n * t * omega), sin(-n * t * omega));
}

FourierSeries::FourierSeries(ComplexFunctionInterface &functionInterface, int32_t _epicyclesCount, int32_t period)
        : f(functionInterface), T(period), epicyclesCount(_epicyclesCount) {
    omega = M_PI * 2 / period;
}

void FourierSeries::calc(List<Epicycle> &list) {
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

void FourierSeries::calc(FourierSeriesCallback &callback) {
    int32_t a = -epicyclesCount / 2;
    int32_t t = a + epicyclesCount;
    ComplexValue c1;
    ComplexDefinite d{};
    for (int32_t n = a; n < t; ++n) {
        tempFunction(c1, t, n);
        ComplexValue df = d.getDefiniteIntegralByTrapezium(0, T, f);
        df.selfDivide(T, 0);
        callback.callback(n, df.re, df.im);
    }
}
