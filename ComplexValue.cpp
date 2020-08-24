//
// Created by root on 2020/4/2.
//

#include "ComplexValue.h"

using namespace bczhc;

double bczhc::ComplexValue::getComplexModule() {
    return sqrt(pow(this->re, 2) + pow(this->im, 2));
}

ComplexValue bczhc::ComplexValue::add(ComplexValue &cv) {
    ComplexValue r(this->re + cv.re, this->im + cv.im);
    return r;
}

bczhc::ComplexValue::ComplexValue(double re, double im) : re(re), im(im) {}

ComplexValue bczhc::ComplexValue::add(double _re, double _im) {
    ComplexValue r(this->re + _re, this->im + _im);
    return r;
}

ComplexValue &bczhc::ComplexValue::selfAdd(ComplexValue &cv) {
    this->re += cv.re;
    this->im += cv.im;
    return *this;
}

ComplexValue &bczhc::ComplexValue::selfAdd(double _re, double _im) {
    this->re += _re;
    this->im += _im;
    return *this;
}

ComplexValue bczhc::ComplexValue::subtract(double _re, double _im) {
    ComplexValue r(this->re - _re, this->im - _im);
    return r;
}

ComplexValue &bczhc::ComplexValue::selfSubtract(ComplexValue cv) {
    this->re -= cv.re;
    this->im -= cv.im;
    return *this;
}

ComplexValue &bczhc::ComplexValue::selfSubtract(double _re, double _im) {
    this->re -= _re;
    this->im -= _im;
    return *this;
}

ComplexValue bczhc::ComplexValue::multiply(ComplexValue cv) {
    ComplexValue r(this->re * cv.re - this->im * cv.im, this->re * cv.im + cv.re * this->im);
    return r;
}

ComplexValue bczhc::ComplexValue::multiply(double _re, double _im) {
    ComplexValue r(this->re * _re - this->im * _im, this->re * _im + _re * this->im);
    return r;
}

ComplexValue &bczhc::ComplexValue::selfMultiply(ComplexValue cv) {
    double re1 = this->re * cv.re - this->im * cv.im;
    double im1 = this->re * cv.im + cv.re * this->im;
    this->re = re1;
    this->im = im1;
    return *this;
}

ComplexValue &bczhc::ComplexValue::selfMultiply(double _re, double _im) {
    double re1 = this->re * _re - this->im * _im;
    double im1 = this->re * _im + _re * this->im;
    this->re = re1;
    this->im = im1;
    return *this;
}

ComplexValue bczhc::ComplexValue::divide(ComplexValue cv) {
    double a = pow(cv.re, 2) + pow(cv.im, 2);
    ComplexValue r((this->re * cv.re + this->im * cv.im) / a, (cv.re * this->im - this->re * cv.im) / a);
    return r;
}

ComplexValue bczhc::ComplexValue::divide(double _re, double _im) {
    double a = pow(this->re, 2) + pow(this->im, 2);
    ComplexValue r((this->re * _re + this->im * _im) / a, (_re * this->im - this->re * _im) / a);
    return r;
}

ComplexValue &bczhc::ComplexValue::selfDivide(ComplexValue cv) {
    double a = pow(cv.re, 2) + pow(cv.im, 2);
    double re1 = (this->re * cv.re + this->im * cv.im) / a;
    double im1 = (cv.re * this->im - this->re * cv.im) / a;
    this->re = re1;
    this->im = im1;
    return *this;
}

ComplexValue &bczhc::ComplexValue::selfDivide(double _re, double _im) {
    double a = pow(_re, 2) + pow(_im, 2);
    double re1 = (this->re * _re + this->im * _im) / a;
    double im1 = (_re * this->im - this->re * _im) / a;
    this->re = re1;
    this->im = im1;
    return *this;
}

ComplexValue &bczhc::ComplexValue::setValue(ComplexValue cv) {
    this->re = cv.re;
    this->im = cv.im;
    return *this;
}

ComplexValue &bczhc::ComplexValue::setValue(double _re, double _im) {
    this->re = _re;
    this->im = _im;
    return *this;
}

ComplexValue bczhc::ComplexValue::subtract(ComplexValue cv) {
    ComplexValue r(this->re - cv.re, im - cv.im);
    return r;
}

string bczhc::ComplexValue::toString() {
    return ComplexValue::toString(this->re, this->im);
}

string bczhc::ComplexValue::toString(ComplexValue cv) {
    return ComplexValue::toString(cv.re, cv.im);
}

string bczhc::ComplexValue::toString(double _re, double _im) {
    if (_re == 0 && _im == 0) {
        string s = "0";
        return s;
    } else if (_re == 0) {
        string s = to_string(_re);
        s.append("i");
        return s;
    } else if (_im == 0) {
        string s = to_string(_re);
        return s;
    } else if (_im > 0) {
        string s = to_string(_re);
        s.append("+")
                .append(to_string(_im))
                .append("i");
        return s;
    } else if (_im < 0) {
        string s = to_string(_re);
        s.append(to_string(_im))
                .append("i");
        return s;
    }
    string s = "0";
    return s;
}
