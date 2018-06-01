#include <iostream>
#include <cmath>
#include <functional>
#include <cassert>
#include <vector>
#include "Grid.h"
#include "Interpolation.h"

using dvector_t = std::vector<double>;
double eps = 1e-4;


double f(double& x, const dvector_t& y) {
    if (x == 0)
        x = 0.0001;
    return pow(y[2], 3) - 2 * sin(x) * y[2] - cos(x) + 5 * log(x) * y[1] + y[0] * pow((x + 3), 2);
}

dvector_t operator *(const dvector_t& vect, double scal) {
    dvector_t result(vect);
    for (double &i : result)
        i *= scal;
    return result;
}

dvector_t operator *(double scal, const dvector_t& vect) {
    return vect * scal;
}

dvector_t operator +(const dvector_t& vect1, const dvector_t& vect2) {
    assert(vect1.size() == vect2.size());
    dvector_t result(vect1);
    for (size_t i = 0; i < result.size(); ++i)
        result[i] += vect2[i];
    return result;
}

double moda(const dvector_t& vect1, const dvector_t& vect2) {
    assert(vect1.size() == vect2.size());
    double max = -1;
    for (size_t i = 0; i < vect1.size(); ++i)
        std::max(max, fabs(vect1[i] - vect2[i]));
    return max;
}

dvector_t F(double x, const dvector_t &y) {
    double a(0), b(2);
    double fa, fb;

    do {
        fa = f(x, dvector_t{y[0], y[1], a});
        fb = f(x, dvector_t{y[0], y[1], b});
        --a;
        ++b;
    } while (fa * fb > 0);

    double c = 0;
    while(fabs(b - a) >= eps) {
        c = (a + b) * 0.5;
        fa = f(x, dvector_t{y[0], y[1], a});
        double fc = f(x, dvector_t{y[0], y[1], c});
        fb = f(x, dvector_t{y[0], y[1], b});
        if (fa * fc < 0) {
            a = c;
        } else if (fc * fb < 0) {
            b = c;
        } else
            break;
    }

    return dvector_t {y[1], (a + b) * 0.5};
}

dvector_t RungeKutte2time(double x, const dvector_t& y, double h) {
    dvector_t y_1_2 = y + h * 0.5 * F(x, y);
    return y + F(x + h * 0.5, y_1_2);
}

double RungeKutte2time_calc(double x0, double x1, double h, const dvector_t &y) {
    size_t n = static_cast<size_t>((x1 - x0) / h) + 1;
    dvector_t Y(y);
    for (size_t i = 0; i < n; ++i)
        Y = RungeKutte2time(x0 + i * h, Y, h);
    return Y[0];
}


double shooting(double x0, double x1, double y0, double y1, double h) {
    double up(1.0), down(0.0);
    double fa, fb;

    do {
        fa = RungeKutte2time_calc(x0, x1, h, dvector_t{y0, up}) - y1;
        fb = RungeKutte2time_calc(x0, x1, h, dvector_t{y0, down}) - y1;
        up -= h;
        down += h;
    } while (fa * fb > 0);

    while (fabs(down - up) > eps) {
        double c = (up + down) * 0.5;
        fa = RungeKutte2time_calc(x0, x1, h, dvector_t{y0, up}) - y1;
        double fc = RungeKutte2time_calc(x0, x1, h, dvector_t{y0, c}) - y1;
        fb = RungeKutte2time_calc(x0, x1, h, dvector_t{y0, down}) - y1;
        if (fa * fc < 0) {
            down = c;
        } else if (fc * fb < 0) {
            up = c;
        } else
            break;
    }
    return (up + down) * 0.5;
}

/*double trapecyIntegrate(const double& a = 0, const double& b = 2, const double& eps = 1e-5) {
    size_t n = 2;
    double h = 0.2;
    double sum = 0., sum1;

    for (size_t i = 1; i <= n - 1; ++i) {
        sum += f(a + h * i);
    }

    sum = ((f(a) + f(b)) / 2 + sum) * h;

    do {
        sum1 = sum;

        h /= 2;
        n *= 2;

        sum = 0.;
        for (size_t i = 1; i <= n - 1; ++i) {
            sum += f(a + h * i);
        }
        sum = ((f(a) + f(b)) / 2 + sum) * h;
    } while (fabs(sum1 - sum) > eps);

    //std::cout << "Trapecy iterations: " << n << std::endl;
    return sum;
}*/


int main(int argc, char** argv) {
    double x0 = 0, x1 = 1;
    double y0 = 2, y1 = 7;
    double h = 0.2;

    double dyp = shooting(x0, x1, y0, y1, h), dy;
    while (fabs(dyp - (dy = shooting(x0, x1, y0, y1, h * 0.5))) > 1e-1) {
        h *= 0.5, dyp = dy;
    }
    std::cout << dyp;

    dvector_t y{ y0, dyp };
    dvector_t _y(6);
    //double y = RungeKutte2time_calc(x0, x1, h, y);
    //auto rungeFunc = &RungeKutte2time_calc;
    dvector_t _x{0.0, 0.2, 0.6, 0.4, 0.8, 1.0 };
    std::unique_ptr<Grid<double>> glut(new Grid<double>(_x, _y, 2));
    glut->initGlut(argc, argv, 1);
    return 0;
}
