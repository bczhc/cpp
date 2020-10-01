//
// Created by root on 2020/3/31.
//

#ifndef BCZHC_FOURIER_SERIES
#define BCZHC_FOURIER_SERIES

#include "./ComplexValue.h"
#include "./Concurrent.h"
#include "./Epicycle.h"
#include "ComplexIntegral.h"
#include "./third_party/practice/LinearList.hpp"
#include <iostream>

using namespace std;
using namespace bczhc;
using namespace linearlist;

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

        //    void calc(ArrayList<Epicycle> &list, int integralD);

        void calc(FourierSeriesCallback &callback, int integralD, int threadNum);
    };

}// namespace bczhc

#endif//BCZHC_FOURIER_SERIES
