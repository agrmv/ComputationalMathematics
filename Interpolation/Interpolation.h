#pragma once
#include <algorithm>
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include <functional>
#include <iostream>
#include <memory>
#include <cmath>

template <class T>
using vector_t = std::vector<T>;

template <class T>
class Interpolation {
protected:
    size_t n;
    vector_t<T> pointsX;
    vector_t<T> pointsY;
    struct Spline {
        double a, b, c, d, x;
    };
public:
    Interpolation() {
        pointsX = {-1.0, -0.6, -0.3, 0.3, 1.0};
        pointsY = { 0.5,  1.0,  0.4, 0.1, 0.5};
        n = (pointsX.size() != pointsY.size()) ? throw std::logic_error("Error vector's size") : pointsX.size();
    }
    Interpolation(std::initializer_list<T> _x, std::initializer_list<T> _y) : pointsX(_x), pointsY(_y) {
        n = (pointsX.size() != pointsY.size()) ? throw std::logic_error("Error vector's size") : pointsX.size();
    }

    Interpolation(const vector_t<T>& _x, const vector_t<T>& _y) : pointsX(_x), pointsY(_y) {
        n = (pointsX.size() != pointsY.size()) ? throw std::logic_error("Error vector's size") : pointsX.size();
    }

    double aitkenInterpolation(const double& x);
    double newtonInterpolation(const double& x);
    double lagrangeInterpolation(const double& x);
    double splineInterpolation(double x);
    void   sweetMethod(vector_t<Spline>& splines);
    double trigonometricInterpolatation(const double &x);

    friend std::ostream &operator<<(std::ostream &os, const vector_t<double> &v) {
        for (const auto &i: v) {
            os << i  << " ";
        }
        return os;
    }
    friend std::istream &operator>>(std::istream &is, vector_t<double> &v) {
        for (auto &i: v) {
            is >> i;
        }
        return is;
    }

};

template<class T>
double Interpolation<T>::aitkenInterpolation(const double &x) {
    vector_t<T> p(n);
    for (size_t i = 0; i < n; ++i) {
        p[i] = pointsY[i];
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = i - 1; k <= i; --k) {
            p[k] = p[k + 1] + (p[k + 1] - p[k])*(x - pointsX[i])/(pointsX[i] - pointsX[k]);
        }
    }
    return p[0];
}

template<class T>
double Interpolation<T>::newtonInterpolation(const double &x) {
    double sum = 0.;
    double w = 1.;
    std::function<double(const int32_t &i, const size_t &k)> f = [&](const int32_t &i, const size_t &k) -> double {
            if (k > 1) {
                return (f(i + 1, k - 1) - f(i, k - 1)) / (pointsX[i + k] - pointsX[i]);
            }
            return k == 0 ? pointsY[i] : (pointsY[i + 1] - pointsY[i]) / (pointsX[i + 1] - pointsX[i]);
    };
    for (size_t i = 0; i < n; i++) {
        sum += w * f(0, i);
        w *= (x - pointsX[i]);
    }
    return sum;
}

template<class T>
double Interpolation<T>::lagrangeInterpolation(const double& x) {
    double res = 0;
    for (size_t i = 0; i <= n; ++i) {
        res += pointsY[i] * ([&](const size_t& n) -> double {
            if (n >= pointsX.size()) {
                return 1;
            }
            double result = 1;
            for (size_t j = 0; j < pointsX.size(); ++j) {
                if (j != n) {
                    result *= (x - pointsX[j]) / (pointsX[n] - pointsX[j]);
                }
            }
            return result;
        })(i);
    }
    return res;
}

template<class T>
void Interpolation<T>::sweetMethod(vector_t<Spline>& splines) {
    vector_t<T> alpha(n - 1);
    vector_t<T> beta(n - 1);

    alpha[0] = beta[0] = 0.0;
    for (size_t i = 1; i < n - 1; i++)	{
        double hi  = pointsX[i] - pointsX[i - 1];
        double hi1 = pointsX[i + 1] - pointsX[i];

        double A = hi;
        double C = 2.0 * (hi + hi1);
        double B = hi1;
        double F = 6.0 * ((pointsY[i + 1] - pointsY[i]) / hi1 - (pointsY[i] - pointsY[i - 1]) / hi);
        double z = (A * alpha[i - 1] + C);

        alpha[i] = -B / z;
        beta[i] = (F - A * beta[i - 1]) / z;
    }

    for (size_t i = n - 2; i > 0; --i) {
        splines[i].c = alpha[i] * splines[i + 1].c + beta[i];
    }
}

template<class T>
double Interpolation<T>::splineInterpolation(double x) {
    vector_t<Spline> splines(n);

    for (size_t i = 0; i < n; i++)	{
        splines[i].x = pointsX[i];
        splines[i].a  = pointsY[i];
    }

    splines[0].c = splines[n - 1].c = 0.0;
    sweetMethod(splines);

    for (size_t i = n - 1; i > 0; i--) {
        double hi = pointsX[i] - pointsX[i - 1];
        splines[i].d = (splines[i].c - splines[i - 1].c) / hi;
        splines[i].b = hi * (2.0 * splines[i].c + splines[i - 1].c) / 6.0 + (pointsY[i] - pointsY[i - 1]) / hi;
    }

    size_t index;

    if (x <= splines[0].x) {
        index = 0;
    }
    else if (x >= splines[n - 1].x) {
        index = n - 1;
    }
    else {
        size_t i = 0;
        size_t j = n - 1;
        while (i + 1 < j) {
            size_t k = (i + j) / 2;

            if (x <= splines[k].x) {
                j = k;
            }
            else {
                i = k;
            }
        }
        index = j;
    }

    double dx = x - splines[index].x;

    return splines[index].a + splines[index].b * dx + splines[index].c * dx * dx / 2 + splines[index].d * dx * dx * dx / 6;
}

template<class T>
double Interpolation<T>::trigonometricInterpolatation(const double& x) {
    auto A = [&](const size_t &j) -> double {
        double S = 0;
        for (size_t k = 0; k < n - 1; ++k) {
            S += pointsY[k] * sin(2 * M_PI * static_cast<double>(k * j) / static_cast<double>(n));
        }
        return ((1 / static_cast<double>(n)) * S);
    };

    auto B = [&](const size_t &j) -> double {
        double S = 0.;
        if (j == 0) {
            for (size_t k = 0; k < n - 1; ++k) {
                S += pointsY[k];
            }
            return ((1 / static_cast<double>(n)) * S);
        }
        for (size_t k = 0; k < n - 1; ++k) {
            S += pointsY[k] * cos(2 * M_PI * static_cast<double>(k * j) / static_cast<double>(n));
        }

        return ((1 / static_cast<double>(n)) * S);
    };

    double y = B(0);
    for (size_t j = 1; j < n; ++j) {
        y += B(j) * cos(2 * M_PI * j * (x - pointsX[0]) / (pointsX[n] - pointsX[0])) +  A(j) * sin(2 * M_PI * j * (x - pointsX[0]) / (pointsX[n] - pointsX[0]));
    }
    return y;
}