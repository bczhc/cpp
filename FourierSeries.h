//
// Created by root on 2020/3/31.
//

#include "./ComplexValue.h"
#include "./Epicycle.h"
#include "./zhc.h"
#include "ComplexIntegral.h"
#include <iostream>
#include <thread>

using namespace std;
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

    //    void calc(ArrayList<Epicycle> &list, int integralD);

    void calc(FourierSeriesCallback &callback, int integralD, int threadNum);
};

class ComplexPointFunction {
private:
    bczhc::ArrayList<ComplexValue> list;

public:
    ComplexValue get(int32_t index);

    void put(ComplexValue &cv);

    // ComplexFunctionInterface getFunction();
};

void tF();
} // namespace bczhc
