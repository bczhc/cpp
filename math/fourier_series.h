//
// Created by root on 2020/3/31.
//

#ifndef BCZHC_FOURIER_SERIES
#define BCZHC_FOURIER_SERIES

#include "./complex_value.h"
#include "../concurrent.h"
#include "./epicycle.h"
#include "../third_party/practice/LinearList.hpp"

using namespace bczhc;

namespace bczhc {
    class FourierSeriesCallback {
    public:
        virtual void callback(double n, double re, double im) = 0;
    };

    class FourierSeries {
    private:
        double omega{};
        double T{};
        ComplexFunctionInterface &f;
        int32_t epicyclesCount{};

    public:
        FourierSeries(ComplexFunctionInterface &functionInterface,
                      int32_t _epicyclesCount, int32_t period);

        void calc(FourierSeriesCallback &callback, int integralD, int threadNum);
    };

    class FuncInIntegralInterface {
    public:
        virtual void x(ComplexValue &dest, double t, int32_t n) = 0;
    };

}// namespace bczhc

#endif//BCZHC_FOURIER_SERIES
