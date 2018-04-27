#include <iostream>
#include <cmath>
#include <functional>

double trapecyIntegrate(const std::function<double(const double&)> &f, const double& a, const double& b, const  double& eps = 1e-5) {
    size_t n = 2;
    double h = (b - a) / n;
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
}

double simpsonIntegrate(const std::function<double(const double&)> &f, const double& a, const double& b, const  double& eps = 1e-5) {
    size_t n = 2;
    double h = (b - a) / n;
    double sum = 0., sum1;
    double x = a + h;

    while (x < b) {
        sum += 4 * f(x);
        x += h;
        sum += 2 * f(x);
        x += h;
    }

    sum = (f(a) + sum - f(b)) * h / 3;

    do {
        sum1 = sum;
        h /= 2;
        n *= 2;
        sum = 0.;
        double x = a + h;

        while (x < b) {
            sum += 4 * f(x);
            x += h;
            sum += 2 * f(x);
            x += h;
        }

        sum = (f(a) + sum - f(b)) * h / 3;
    } while (fabs(sum1 - sum) > eps);

    //std::cout << "Simpson iterations: " << n << std::endl;
    return sum;
}

/*  double x0 = a;
    double x1 = a + h;

    for (size_t i = 0; i <= n - 1; ++i) {
        sum += f(x0) + 4 * f(x0 + h / 2) + f(x1);

        x0 += h;
        x1 += h;
    }

    return (h / 6) * sum;
*/


int main() {
    auto f = [](const double &x) {
        return log(1 + x) / x;
    };
    double a, b;
    size_t eps;
    std::cout << "Enter a: ";
    std::cin >> a;
    std::cout << "Enter b: ";
    std::cin >> b;
    std::cout << "Enter eps: ";
    std::cin >> eps;
    std::cout << "trapecyIntegrate: " << trapecyIntegrate(f, a, b, eps) << std::endl;
    std::cout << "simpsonIntegrate: " << simpsonIntegrate(f, a, b, eps) << std::endl;
    return 0;
}