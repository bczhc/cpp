//
// Created by root on 2020/3/31.
//

#include "./FourierSeries.h"
#include "ComplexIntegral.h"
#include "zhc.h"

using namespace bczhc;
using namespace concurrent;

bczhc::FourierSeries::FourierSeries(ComplexFunctionInterface &functionInterface,
                                    int32_t _epicyclesCount, int32_t period)
        : f(functionInterface), T(period), epicyclesCount(_epicyclesCount) {
    omega = M_PI * 2 / period;
}

void bczhc::FourierSeries::calc(FourierSeriesCallback &callback, int integralD,
                                int threadNum) {
    LongWaitCountDownLatch latch(epicyclesCount);
    ThreadPool *pool = Executors::newFixedThreadPool(threadNum);
    PointersSet deletable;
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
        LongWaitCountDownLatch &latch;

    public:
        Run(FuncInIntegral &funcInIntegral, ComplexIntegral &complexIntegral,
            FourierSeriesCallback &callback, int32_t n, double T,
            LongWaitCountDownLatch &latch)
                : funcInIntegral(funcInIntegral), complexIntegral(complexIntegral),
                  callback(callback), n(n), T(T), latch(latch) {}

        void run() override {
            ComplexValue integralResult =
                    complexIntegral.getDefiniteIntegralByTrapezium(0, T, funcInIntegral, n);
            integralResult.selfDivide(T, 0);
            callback.callback(n, integralResult.re, integralResult.im);
            latch.countDown();
        }
    };
    for (int32_t n = a; n < t; ++n) {
        Run *runnable =
                new Run(funcInIntegral, complexIntegral, callback, n, T, latch);
        deletable.put(runnable);
        pool->execute(runnable);
    }
    latch.wait();
    free(pool);
    deletable.freeAll();
}
