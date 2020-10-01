#include <iostream>
#include "../third_party/practice/LinearList.hpp"
#include "../FourierSeries.h"

using namespace std;
using namespace bczhc;
using namespace linearlist;

class Point {
public:
    float x, y;

    Point() = default;

    Point(float x, float y) : x(x), y(y) {}
};


class F : public ComplexFunctionInterface {
private:
    SequentialList<Point> &list;
    double period, scale{};
    double pathsTotalLength{};
    double *sumLength = nullptr;
    int listLength;

    static double getPathLength(Point &p1, Point &p2) {
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }

    static int search(const double *arr, int length, double target) {
        for (int i = 0; i < length - 1; ++i) {
            if (target > arr[i] && target < arr[i + 1]) return i;
        }
        if (target > arr[length - 1]) return length - 1;
        return -1;
    }

    static Point linearMoveBetweenTwoPoints(Point p1, Point p2, double progress) {
        Point p((float) (p1.x + (p2.x - p1.x * progress)), (float) (p1.y + (p2.y - p1.y) * progress));
        return p;
    }

public:

    explicit F(SequentialList<Point> &list, double period) : list(list), period(period) {
        listLength = list.length();
        sumLength = new double[listLength];
        for (int i = 0; i < listLength; ++i) {
            Point next = i == listLength - 1 ? list.get(0) : list.get(i + 1);
            Point currPoint = list.get(i);
            pathsTotalLength += getPathLength(currPoint, next);
            sumLength[i] = pathsTotalLength;
        }
    }

    ~F() {
        delete[] sumLength;
    }

    void x(ComplexValue &dest, double t) override {
        t = t - (t >= period ? floor(t / period) * period : 0);
        double mapToLength = t * pathsTotalLength / period;
        int index = search(sumLength, listLength, mapToLength);
        Point r = linearMoveBetweenTwoPoints(list.get(index),
                                             index == listLength - 1 ? list.get(0) : list.get(index + 1),
                                             mapToLength - sumLength[index]);
        dest.re = r.x, dest.im = r.y;
    }
};

int main(int argc, char **argv) {
    SequentialList<Point> list;
    Point point(0, 0);
    list.insert(point);
    point.x = 0, point.y = 1;
    list.insert(point);
    point.x = 1, point.y = 0;
    list.insert(point);

    class Callback : public FourierSeriesCallback {
    public:
        void callback(double n, double re, double im) override {
            cout << n << ' ' << re << ' ' << im << endl;
        }
    } callback;
    F f(list, 10);
    FourierSeries fs(f, 100, 10);
    fs.calc(callback, 1000000, 4);
    return 0;
}