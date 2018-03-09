#include <iostream>
#include <cmath>
#include <functional>

double MethodDichotomy(const std::function<double(double)> &myFunction, double a, double b, const double& epsilon = 1e-5) {
    double x = (a + b) / 2;
    while (fabs(b - a) > epsilon)
        if (myFunction(x) * myFunction(a) < 0) {
            b = x;
            x = (a + b) / 2;
        }
        else if (myFunction(x) * myFunction(b) < 0) {
            a = x;
            x = (a + b) / 2;
        }
    return x;
}

double MethodChord(const std::function<double(double)> &myFunction, double a, double b, const double& epsilon = 1e-5) {
    double x;
    while (fabs(b - a) > epsilon) {
        a = b - (b - a) * myFunction(b) / (myFunction(b) - myFunction(a));
        b = a - (a - b) * myFunction(a) / (myFunction(a) - myFunction(b));
    }
    return a;
}


double NewtonsMethod(const std::function<double(double)> &fx, const std::function<double(double)> &dfx, double x0, const double& epsilon = 1e-5) {
    double x1  = x0 - fx(x0) / dfx(x0);         // первое приблжение
    while (fabs(x1 - x0) > epsilon) {           // пока не достигнута точность
        x0 = x1;
        x1 = x1 - fx(x1) / dfx(x1);             // последующие приближения
    }
    return x1;
}

int main() {
    auto myFunction = [](double x) -> auto{
        return pow(x, 3) - 2 * pow(x, 2) - 6 * x - 1;
    };
    double a = 3, b = 4;

    std::cout << "Методы\n1 - Половинного деления\n2 - Хорд\n3 - Ньютона" << std::endl;
    uint16_t method;
    std::cin >> method;
    switch(method) {
        case 1: {
            std::cout << MethodDichotomy(myFunction, a, b) << std::endl;
            break;
        }
        case 2: {
            std::cout << MethodChord(myFunction, a, b) << std::endl;
            break;
        }
        case 3: {
            auto MyDerivative = [&myFunction](double x, double h = 0.1) -> auto {
                return (myFunction(x + h) - myFunction(x - h)) / (2 * h);
            };

            auto My2Derivative = [&myFunction](double x, double h = 0.1) -> auto {
                return (myFunction(x + h) - 2 * myFunction(x) + myFunction(x - h)) / (h * h);
            };
            std::cout << NewtonsMethod(myFunction, MyDerivative, 3) << std::endl;
            break;
        }
        default:break;
    }
    return 0;
}