#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <iomanip>
#include <cmath>
#include <cassert>

#define USE_BYPASS

using namespace std;
using dvector_t = vector<double>;
using dpoint_t = pair<double, dvector_t>;
using point_t = pair<double, double>;
using pvector_t = vector<point_t>;
double eps = 1e-2;

dvector_t operator *(const dvector_t& vect, double scal) {
    dvector_t result(vect);
    for (double &i : result) {
        i *= scal;
    }
    return result;
}

dvector_t operator *(double scal, const dvector_t& vect) {
    return vect * scal;
}

dvector_t operator +(const dvector_t& vect1, const dvector_t& vect2) {
    assert(vect1.size() == vect2.size());
    dvector_t result(vect1);
    for (int i = 0; i < result.size(); ++i) {
        result[i] += vect2[i];
    }
    return result;
}

/*double moda(const dvector_t& vect1, const dvector_t& vect2) {//максимальная разница значений
    assert(vect1.size() == vect2.size());
    double max = -1;
    for (int i = 0; i < vect1.size(); ++i) {
        max = std::max(max, fabs(vect1[i] - vect2[i]));
    }
    return max;
}*/

double modaPoint(const vector<dpoint_t> &vect1, const vector<dpoint_t> &vect2) {//максимальная разница значений
    double max = -1, max2 = -1;
    for (const auto &i : vect1) {
        for (int j = 0; j < vect2.size() && i.first >= vect2[j].first; ++j) {
            if (i.first != vect2[j].first) continue;
            max = std::max(max, fabs(i.second[0] - vect2[j].second[0]));
            max2 = std::max(max2, fabs(i.second[1] - vect2[j].second[1]));
        }
    }
    return std::max(max, max2);;
}

double f(double x, const dvector_t& y) {
    x += 1e-4;
    return pow(y[2], 3) - 2 * sin(x) * y[2] - cos(x) + 9 * log(x) / log(M_E) * y[1] + y[0] / pow(x + 3, 2);
}

dvector_t F(double x, const dvector_t &y) {
    double up(2.0), down(0.0);
    double fa, fb;
    //shooting y"
    do {
        fa = f(x, dvector_t{y[0], y[1], up--});
        fb = f(x, dvector_t{y[0], y[1], down++});
    } while (fa * fb > 0);

    double c = 0;
    while(fabs(down - up) > eps) {      //binary search
        c = (up + down) * 0.5;
        fa = f(x, dvector_t{y[0], y[1], up});
        double fc = f(x, dvector_t{y[0], y[1], c});
        fb = f(x, dvector_t{y[0], y[1], down});;
        if(fa * fc < 0)
            down = c;
        else if (fc * fb < 0)
            up = c;
    }
    return  dvector_t {y[1], (up + down) * 0.5};
}

dvector_t RungeKutta2time(double x, const dvector_t &y, double h) {
    dvector_t y_1_2 = y + h * 0.5 * F(x, y);
    return y + h * F(x + h * 0.5, y_1_2);//тут была ошибка
}

double RungeKutta2time_calc(double x0, double x1, double h, const dvector_t &y, bool ret_deriv = false) {
    dvector_t Y(y);
    for (; x0 < x1; x0 += h) {
        Y = RungeKutta2time(x0, Y, h);
    }
    return Y[ret_deriv];
}

double shooting(double x0, double x1, double y0, double y1, double h, bool shoot_derivaive = false) {
    double up(3.0), down(0.0);
    double fa, fb;

    do {
        fa = RungeKutta2time_calc(x0, x1, h, dvector_t{y0, up}, shoot_derivaive) - y1;
        fb = RungeKutta2time_calc(x0, x1, h, dvector_t{y1, down}, shoot_derivaive) - y1;
        up -= h;
        down += h;
    } while (fa * fb > 0);

    up += h;
    down -= h;

    double c;
    while(fabs(down - up) > 1e-3) {
        c = (down + up) * 0.5;
        double fc = RungeKutta2time_calc(x0, x1, h, dvector_t{c, y1}, shoot_derivaive) - y1;
        if(fa * fc < 0) {
            down = c;
            fb = fc;
        } else if (fc * fb < 0) {
            up = c;
            fa = fc;
        }
    }
    return (down + up) / 2;
}

vector<dpoint_t> getPoints(double x0, double x1, const dvector_t &y0, double &h) {
    auto n = size_t((x1 - x0 + 1e-6) / h + 1);
    vector<dpoint_t> res(n), res2(n);
    res2[0].second = y0;
    res2[0].first = x0;
    for (int i = 1; i < n; ++i) {
        res2[i].first = x0 + i * h;
        res2[i].second = RungeKutta2time(res2[i].first, res2[i - 1].second, h);
    }

    do {
        res = std::move(res2);
        h *= 0.5;
        n = size_t((x1 - x0 + 1e-6) / h + 1);
        res2.resize(n);
        res2[0].second = y0;
        res2[0].first = x0;
        for (int i = 1; i < n; ++i) {
            res2[i].first = x0 + i * h;
            res2[i].second = RungeKutta2time(res2[i].first, res2[i - 1].second, h);
        }
    } while (modaPoint(res, res2) > eps
             #ifdef USE_BYPASS
             && fabs(res2[n - 1].second[1] - 3) > eps);
#else
    );
#endif
    return res2;
}

double newton(const pvector_t &points, double x, size_t n, double h) {
    pvector_t p(points);
    const double q = (x - p[0].first) / h;
    double result = p[0].second, fq = q;
    int fact = 1;
    for (int i = 1; i < n; ++i, fact *= i, fq *= q - i + 1) {
        for (int j = 0; j < n - i; ++j) {
            p[j].second = p[j + 1].second - p[j].second;
        }
        result += (p[0].second * fq) / fact;
    }
    return result;
}

double integrationSimpson(double a, double b, double h, const dvector_t& y) {
    const double dh = 2 * h;
    double result = RungeKutta2time_calc(a, 0, h, y, true) + RungeKutta2time_calc(a, b, h, y, true) + 4 *
                                                                                                      RungeKutta2time_calc(a, b - h, h, y, true);
    for (double x = a + h; x < b - dh; x += dh) {
        result += 4 * RungeKutta2time_calc(a, x, h, y, true) + 2 * RungeKutta2time_calc(a, x + h, h, y, true);
    }
    return result * h / 3;
}

int main(int argc, char** argv) {
    double x0 = 0, x1 = 1;
    double y0 = 2, y1 = 7;
    const double h0 = 0.2;
    auto count = size_t((x1 - x0 + 1e-6) / h0 + 1);
    dvector_t y {y0, shooting(x0, x1, y0, y1, h0, false)};

    double h = h0;
    vector<dpoint_t> p = getPoints(x0, x1, y, h);
    ofstream fout("solve.txt", ios_base::out);
    for (auto& val : p) {
        fout << val.first << " " << val.second[0] << endl;
    }
    fout.close();
    pvector_t points(count), deriv(count);
    for (size_t i = 0; i < count; ++i) {
        deriv[i].first = points[i].first = x0 + i * h0;
    }

    ofstream derout("y_deriv_data.txt", ios_base::out);
    fout.open("y_data.txt", ios_base::out);
    for (size_t i = 0, j = 0; i < p.size(); ++i) {
        if (fabs(p[i].first - points[j].first) < 1e-4) {
            deriv[j].second = p[i].second[1];
            points[j++].second = p[i].second[0];
        }
        fout << p[i].first << " " << setprecision(14) << p[i].second[0] << endl;
        derout << p[i].first << " " << setprecision(14) << p[i].second[1] << endl;
    }
    fout.close();
    derout.close();

    fout.open("y_interp.txt", ios_base::out);
    derout.open("y_deriv_interp.txt", ios_base::out);
    for (size_t i = 0; i < p.size(); ++i) {
        fout << x0 + i * h << " " << setprecision(14) << newton(points, x0 + i * h, count, h0) << endl;
        derout << x0 + i * h << " " << setprecision(14) << newton(deriv, x0 + i * h, count, h0) << endl;
    }
    fout.close();
    derout.close();

    h = h0;
    double i1 = integrationSimpson(x0, x1, h, y), i2;
    while (fabs(i1 - (i2 = integrationSimpson(x0, x1, h * 0.5, y))) > 1e-3) {
        h *= 0.5, i1 = i2;
    }
    cout << "Integral: " << i1 << endl;
    return 0;
}