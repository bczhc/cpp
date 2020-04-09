//
// Created by root on 2020/3/31.
//

#include "./FourierSeries.h"

FourierSeries::FourierSeries(ComplexFunctionInterface &functionInterface, int32_t _epicyclesCount, int32_t period)
        : f(functionInterface), T(period), epicyclesCount(_epicyclesCount) {
    omega = M_PI * 2 / period;
}

void FourierSeries::calc(ArrayList<Epicycle> &list) {

}

void FourierSeries::calc(FourierSeriesCallback &callback) {
    int32_t a = -epicyclesCount / 2;
    int32_t t = a + epicyclesCount;
    class FuncInIntegral : public ComplexFunctionInterface {
    private:
        ComplexFunctionInterface &mF;
        double &mOmega;
    public:
        double n{};

        FuncInIntegral(ComplexFunctionInterface &mF, double &mOmega) : mF(mF), mOmega(mOmega) {}

    private:
        void x(ComplexValue &dest, double t) override {
            mF.x(dest, t);
            dest.selfMultiply(cos(n * t * mOmega), -sin(n * t * mOmega));
        }
    } funcInIntegral(f, omega);
    ComplexIntegral complexIntegral{.n = 100};
    for (int32_t n = a; n < t; ++n) {
        funcInIntegral.n = n;
        ComplexValue integralResult = complexIntegral.getDefiniteIntegralByTrapezium(0, T, funcInIntegral);
        integralResult.selfDivide(T, 0);
        callback.callback(n, integralResult.re, integralResult.im);
    }
}

int main() {
    class F : public ComplexFunctionInterface {
    public:
        void x(ComplexValue &dest, double t) override {
//            dest.setValue(cos(t), sin(t));
//            dest.selfMultiply(10, 0);
            dest.setValue(10, 10);
        }
    } f;
    FourierSeries fs(f, 10, 50);
    class CB : public FourierSeriesCallback {
    public:
        void callback(double n, double re, double im) override {
            cout << "n: " << n << " re: " << re << " im: " << im << endl;
        }
    } cb;
    fs.calc(cb);
    class F2 : public ComplexFunctionInterface {
    private:
        ComplexFunctionInterface &f1;
    public:
        explicit F2(ComplexFunctionInterface &f1) : f1(f1) {}

    public:
        void x(ComplexValue &dest, double t) override {
            f1.x(dest, t);
            dest.selfMultiply(cos(0 * -.1 * t), sin(0 * -0.1 * t));
        }
    } f2(f);
    ComplexIntegral complexIntegral{.n = 10000};
    ComplexValue r = complexIntegral.getDefiniteIntegralByTrapezium(0, 2 * M_PI, f);
    cout << r.re << endl;
}