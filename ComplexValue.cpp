//
// Created by root on 2020/4/2.
//

#include "ComplexValue.h"

double ComplexValue::getComplexModule() {
    return sqrt(pow(this->re, 2) + pow(this->im, 2));
}

ComplexValue ComplexValue::add(ComplexValue &cv) {
    ComplexValue r(this->re + cv.re, this->im + cv.im);
    return r;
}

ComplexValue::ComplexValue(double re, double im) : re(re), im(im) {}

ComplexValue ComplexValue::add(double _re, double _im) {
    ComplexValue r(this->re + _re, this->im + _im);
    return r;
}

ComplexValue &ComplexValue::selfAdd(ComplexValue &cv) {
    this->re += cv.re;
    this->im += cv.im;
    return *this;
}

ComplexValue &ComplexValue::selfAdd(double _re, double _im) {
    this->re += _re;
    this->im += _im;
    return *this;
}

ComplexValue ComplexValue::subtract(double _re, double _im) {
    ComplexValue r(this->re - _re, this->im - _im);
    return r;
}

ComplexValue &ComplexValue::selfSubtract(ComplexValue cv) {
    this->re -= cv.re;
    this->im -= cv.im;
    return *this;
}

ComplexValue &ComplexValue::selfSubtract(double _re, double _im) {
    this->re -= _re;
    this->im -= _im;
    return *this;
}

ComplexValue ComplexValue::multiply(ComplexValue cv) {
    ComplexValue r(this->re * cv.re - this->im * cv.im, this->re * cv.im + cv.re * this->im);
    return r;
}

ComplexValue ComplexValue::multiply(double _re, double _im) {
    ComplexValue r(this->re * _re - this->im * _im, this->re * _im + _re * this->im);
    return r;
}

ComplexValue &ComplexValue::selfMultiply(ComplexValue cv) {
    this->re = this->re * cv.re - this->im * cv.im;
    this->im = this->re * cv.im + cv.re * this->im;
    return *this;
}

ComplexValue &ComplexValue::selfMultiply(double _re, double _im) {
    this->re = this->re * _re - this->im * _im;
    this->im = this->re * _im + _re * this->im;
    return *this;
}

ComplexValue ComplexValue::divide(ComplexValue cv) {
    double a = pow(cv.re, 2) + pow(cv.im, 2);
    ComplexValue r((this->re * cv.re + this->im * cv.im) / a, (cv.re * this->im - this->re * cv.im) / a);
    return r;
}

ComplexValue ComplexValue::divide(double _re, double _im) {
    double a = pow(this->re, 2) + pow(this->im, 2);
    ComplexValue r((this->re * _re + this->im * _im) / a, (_re * this->im - this->re * _im) / a);
    return r;
}

ComplexValue &ComplexValue::selfDivide(ComplexValue cv) {
    double a = pow(cv.re, 2) + pow(cv.im, 2);
    this->re = (this->re * cv.re + this->im * cv.im) / a;
    this->im = (cv.re * this->im - this->re * cv.im) / a;
    return *this;
}

ComplexValue &ComplexValue::selfDivide(double _re, double _im) {
    double a = pow(_re, 2) + pow(_im, 2);
    this->re = (this->re * _re + this->im * _im) / a;
    this->im = (_re * this->im - this->re * _im) / a;
    return *this;
}

ComplexValue &ComplexValue::setValue(ComplexValue cv) {
    this->re = cv.re;
    this->im = cv.im;
    return *this;
}

ComplexValue &ComplexValue::setValue(double _re, double _im) {
    this->re = _re;
    this->im = _im;
    return *this;
}

ComplexValue ComplexValue::subtract(ComplexValue cv) {
    ComplexValue r(this->re - cv.re, im - cv.im);
    return r;
}
