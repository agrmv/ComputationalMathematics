#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <functional>
#include <cassert>
#include <algorithm>
#include "header.h"

using funcN = std::function<double(const vector_t&)>;

double derivative(const funcN& f, const vector_t& x, size_t& xN, double dx = 1e-6) {
    assert(f);
    assert(xN < x.size());
    vector_t xr(x.size(), 0), xl(x.size(), 0);
    xr[xN] = x[xN] + dx;
    xl[xN] = x[xN] - dx;
    return (f(xr) - f(xl)) / 2 / dx;
}

void JacobianMatrix(const std::vector<funcN>& system, const vector_t& values, matrix_t& matrix) {
    size_t equation, variable;
    for (equation = 0; equation < system.size(); ++equation) {
        auto& _equation = system[equation];
        for (variable = 0; variable < values.size(); ++variable)
            matrix[equation][variable] = derivative(_equation, values, variable);
        matrix[equation][variable] = _equation(values);
    }
}

void newtonsMethod(const std::vector<funcN>& system, vector_t& values, double epsilon = 1e-5) {
    vector_t x(values.size());
    matrix_t w(system.size());
    for (auto& row : w) {
        row.resize(values.size() + 1);
    }
    do {
        x = values;
        JacobianMatrix(system, values, w);
        values = x - NonlinearEquations().calculateGauss(w);
    } while (fabs(NonlinearEquations().getNorm(values - x)) > epsilon);
}

int main() {
    std::vector<funcN> functions;
    auto f0 = [](const vector_t& x) { return x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - 1; };
    auto f1 = [](const vector_t& x) { return 2 * x[0] * x[0] + x[1] * x[1] - 4 * x[2]; };
    auto f2 = [](const vector_t& x) { return 3 * x[0] * x[0] - 4 * x[1] + x[2] * x[2]; };
    functions.emplace_back(f0);
    functions.emplace_back(f1);
    functions.emplace_back(f2);
    vector_t x{0.5, 0.5, 0.5};
    newtonsMethod(functions, x);
    for (size_t index = 0; index < x.size(); ++index) {
        std::cout << "X" << index << "= " << x[index] << std::endl;
    }
    std::cout << "Test:" << std::endl;
    for (size_t index = 0; index < functions.size(); ++index) {
        std::cout << "F" << index << "= " << functions[index](x) << std::endl;
    }
    return 0;
}