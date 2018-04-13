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

    double aitkenInterpolation(const double& _x);
    double f(const int32_t& i, const size_t& k);
    double newtonInterpolation(const double& _x);
    double q(const vector_t<T> &v, const double& x, size_t n);
    double lagrangeInterpolation(const double& _x);
    double splineInterpolation(double x);
    void sweetMethod(vector_t<Spline>& splines);

    friend std::ostream& operator<< (std::ostream& os, const vector_t<T>& v);
    friend std::istream& operator>> (std::istream& is, vector_t<T>& v);
};

template<class T>
double Interpolation<T>::aitkenInterpolation(const double &_x) {
    vector_t<T> p(n);
    for (size_t i = 0; i < n; ++i) {
        p[i] = pointsY[i];
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = i - 1; k <= i; --k) {
            p[k] = p[k + 1] + (p[k + 1] - p[k])*(_x - pointsX[i])/(pointsX[i] - pointsX[k]);
        }
    }
    return p[0];
}

template<class T>
double Interpolation<T>::f(const int32_t &i, const size_t &k) {
    if (k > 1) {
        return (f(i + 1, k - 1) - f(i, k - 1)) / (pointsX[i + k] - pointsX[i]);
    }
    return k == 0 ? pointsY[i] : (pointsY[i + 1] - pointsY[i]) / (pointsX[i + 1] - pointsX[i]);
}

template<class T>
double Interpolation<T>::newtonInterpolation(const double &_x) {
    double sum = 0.;
    double w = 1.;
    for (size_t i = 0; i < n; i++) {
        sum += w * f(0, i);
        w *= (_x - pointsX[i]);
    }
    return sum;
}

template<class T>
double Interpolation<T>::q(const vector_t<T> &v, const double& x, size_t n) {
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
double Interpolation<T>::lagrangeInterpolation(const double& _x) {
    double res = 0;
    for (size_t i = 0; i <= n; ++i) {
        res += pointsY[i] * q(pointsX, _x, i);
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

template<class T>
void Interpolation<T>::sweetMethod(vector_t<Spline>& splines) {
    vector_t<T> alpha(n - 1);
    vector_t<T> beta(n - 1);

    alpha[0] = beta[0] = 0.0;
    //Вычисление прогоночных коэффициентов - прямой ход метода прогонки
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

    // нахождение решения - обратный ход метода прогонки
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
    // Решение СЛАУ относительно коэффициентов сплайнов c[i] методом прогонки для трехдиагональных матриц
    sweetMethod(splines);

    // по известным коэффициентам c[i] находим значения b[i] и d[i]
    for (size_t i = n - 1; i > 0; i--) {
        double hi = pointsX[i] - pointsX[i - 1];
        splines[i].d = (splines[i].c - splines[i - 1].c) / hi;
        splines[i].b = hi * (2.0 * splines[i].c + splines[i - 1].c) / 6.0 + (pointsY[i] - pointsY[i - 1]) / hi;
    }

    size_t index;

    if (x <= splines[0].x) {
        index = 0; // если pointsX меньше точки сетки pointsX[0] - пользуемся первым сплайном
    }
    else if (x >= splines[n - 1].x) {
        index = n - 1; // если pointsX больше точки сетки pointsX[n - 1] - пользуемся последним сплайном
    }
    else {
        // Иначе pointsX лежит между граничными точками сетки - производим бинарный поиск нужного эл-та массива
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