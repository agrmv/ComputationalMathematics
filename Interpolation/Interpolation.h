#pragma once
#include <algorithm>
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include <functional>
#include <iostream>
#include <memory>

template <class T>
using vector_t = std::vector<T>;

template <class T>
class Interpolation {
public:
    size_t n;
    vector_t<T> x;
    vector_t<T> y;
public:
    Interpolation() {
        x = {-1.0, -0.6, -0.3, 0.3, 1.0};
        y = { 0.5,  1.0,  0.4, 0.1, 0.5};
        if (x.size() != y.size()) {
            throw std::logic_error("Error vector's size");
        }
        n = x.size();
    }
    Interpolation(std::initializer_list<T> _x, std::initializer_list<T> _y) : x(_x), y(_y) {
        if (x.size() != y.size()) {
            throw std::logic_error("Error vector's size");
        }
        n = x.size();
    }
    const double aitkenInterpolation(const double& _x);
    const double f(const int32_t& i, const size_t& k);
    const double newtonInterpolation(const double& _x);
    const double q(const vector_t<T> &v, double x, size_t n);
    const double lagrangeInterpolation(double _x);
    friend std::ostream& operator<< (std::ostream& os, const vector_t<T>& v);
    friend std::istream& operator>> (std::istream& is, vector_t<T>& v);
};

template<class T>
const double Interpolation<T>::aitkenInterpolation(const double &_x) {
    vector_t<T> p(n);
    for (size_t i = 0; i < n; ++i) {
        p[i] = y[i];
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = i - 1; k <= i; --k) {
            p[k] = p[k + 1] + (p[k + 1] - p[k])*(_x - x[i])/(x[i] - x[k]);
        }
    }

    return p[0];
}

template<class T>
const double Interpolation<T>::f(const int32_t &i, const size_t &k) {
    if (k > 1) {
        return (f(i + 1, k - 1) - f(i, k - 1)) / (x[i + k] - x[i]);
    }
    return k == 0 ? y[i] : (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
}

template<class T>
const double Interpolation<T>::newtonInterpolation(const double &_x) {
    double sum = 0.;
    double w = 1.;
    for (size_t i = 0; i < n; i++) {
        sum += w * f(0, i);
        w *= (_x - x[i]);
    }
    return sum;
}

template<class T>
const double Interpolation<T>::q(const vector_t<T> &v, double x, size_t n) {
    if (n >= v.size()) {
        return 1;
    }
    double result = 1;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i != n) {
            result *= (x - v[i]) / (v[n] - v[i]);
        }
    }
    return result;
}

template<class T>
const double Interpolation<T>::lagrangeInterpolation(double _x) {
    double res = 0;
    for (size_t i = 0; i <= n; ++i) {
        res += y[i] * q(x, _x, i);
    }
    return res;
}

template<class T>
std::ostream &operator<<(std::ostream &os, const vector_t<T> &v) {
    for (const auto &i: v) {
        os << i  << " ";
    }
    return os;
}

template<class T>
std::istream &operator>>(std::istream &is, vector_t<T> &v) {
    for (auto &i: v) {
        is >> i;
    }
    return is;
}