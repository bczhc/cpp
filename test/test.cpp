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

    static double getPathLength(Point &p1, Point &p2) {
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }

public:

    explicit F(SequentialList<Point> &list, double period) : list(list), period(period) {
        int len = list.length();
        sumLength = new double[len];
        for (int i = 0; i < len; ++i) {
            Point next = i == len - 1 ? list.get(0) : list.get(i + 1);
            Point currPoint = list.get(i);
            pathsTotalLength += getPathLength(currPoint, next);
            sumLength[i] = pathsTotalLength;
        }
    }

    ~F() {
        delete[] sumLength;
    }

    void x(ComplexValue &dest, double t) override {
        double mapToLength = t * pathsTotalLength / period;

    }
};

int binarySearch(const double *arr, int length, int target) {
    int p = 0, q = length - 1, mid;
    while (p <= q) {
        mid = (p + q) / 2;
        if (arr[mid] > target) q = mid - 1;
        else if (arr[mid] < target) p = mid + 1;
        else return mid;
    }
    return -1;
}

int search(const double *arr, int length, double target) {
    for (int i = 0; i < length - 1; ++i) {
        if (target > arr[i] && target < arr[i + 1]) return i;
    }
    if (target > arr[length - 1]) return length - 1;
    return -1;
}

int rangeBinarySearch(const double *arr, int length, double target) {
    int p = 0, q = length - 1, mid;
    while (p <= q) {
        mid = (p + q) / 2;
        if (target > arr[mid] && target > arr[mid + 1]) p = mid + 1;
        else if (target < arr[mid] && target < arr[mid - 1]) q = mid - 1;
        else if (target < arr[mid]) return mid - 1;
        else return mid;
    }
    return -1;
}

int main(int argc, char **argv) {
    //              0, 1, 2, 3, 4, 5, 6
    double arr[] = {1, 2, 5, 6, 8, 9, 10};
    int index = rangeBinarySearch(arr, 7, 8);
    cout << index << endl << endl;
    for (int i = 0; i <= 11; ++i) {
        cout << i << ' ' << search(arr, 7, i) << ' ' << rangeBinarySearch(arr, 7, i) << endl;
    }
    return 0;
    SequentialList<Point> list;
    Point point(0, 0);
    list.insert(point);
    point.x = 0, point.y = 1;
    list.insert(point);
    point.x = 1, point.y = 0;
    list.insert(point);

    F f(list, 10);
    ComplexValue cv;
    f.x(cv, 5);
    cout << cv.re << ' ' << cv.im << endl;
    return 0;
}