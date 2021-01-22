//
// Created by root on 2020/3/31.
//

#include "./fourier_series.h"
#include "complex_integral.h"

using namespace bczhc;
using namespace concurrent;

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

bczhc::FourierSeries::FourierSeries(ComplexFunctionInterface &functionInterface,
                                    int32_t _epicyclesCount, int32_t period)
        : f(functionInterface), T(period), epicyclesCount(_epicyclesCount) {
    omega = M_PI * 2 / period;
}

void bczhc::FourierSeries::calc(FourierSeriesCallback &callback, int integralD,
                                int threadNum) {
    ThreadPool *pool = Executors::newFixedThreadPool(threadNum);
    int32_t a = -epicyclesCount / 2;
    int32_t t = a + epicyclesCount;
    class FuncInIntegral : public FuncInIntegralInterface {
    private:
        ComplexFunctionInterface &mF;
        double mOmega;

    public:
        FuncInIntegral(ComplexFunctionInterface &mF, double &mOmega)
                : mF(mF), mOmega(mOmega) {}

    private:
        void x(ComplexValue &dest, double t, int32_t n) override {
            mF.x(dest, t);
            dest.selfMultiply(cos(-n * t * mOmega), sin(-n * t * mOmega));
        }
    } funcInIntegral(f, omega);
    ComplexIntegral complexIntegral{.integralN = integralD};
    class Run : public Runnable {
    private:
        FuncInIntegral &funcInIntegral;
        ComplexIntegral &complexIntegral;
        FourierSeriesCallback &callback;
        int32_t n;
        double T;

    public:
        Run(FuncInIntegral &funcInIntegral, ComplexIntegral &complexIntegral,
            FourierSeriesCallback &callback, int32_t n, double T)
                : funcInIntegral(funcInIntegral), complexIntegral(complexIntegral),
                  callback(callback), n(n), T(T) {}

        void run() override {
            ComplexValue integralResult =
                    complexIntegral.getDefiniteIntegralByTrapezium(0, T, funcInIntegral, n);
            integralResult.selfDivide(T, 0);
            callback.callback(n, integralResult.re, integralResult.im);
            delete this;
        }
    };
    for (int32_t n = a; n < t; ++n) {
        Run *runnable =
                new Run(funcInIntegral, complexIntegral, callback, n, T);
        pool->execute(runnable);
    }
    
    delete pool;
}
