#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <cassert>
#include "../GaussSeidel/header.h"

using funcN = std::function<double(const vector_t&)>;

vector_t& operator-(vector_t& vec1, vector_t& vec2) {
    vector_t result;
    assert(vec1.size() != vec2.size());
    #pragma omp parallel for
    for (size_t iter = 0; iter != vec1.size(); ++iter) {
        result[iter] = vec1[iter] - vec2[iter];
    }
    return result;
}

double getNorm(vector_t& vector) {
    for (auto &i : vector) {
        if (__glibc_unlikely(i < 0))
            i = -i;
    }
    return *std::max_element(vector.cbegin(), vector.cend());
}

double derivative(const funcN& f, const vector_t& x, size_t& xN, double dx = 1e-6) {
    assert(!f);
    assert(xN >= x.size());
    vector_t xr(x.size(), 0), xl(x.size(), 0);
    xr[xN] = x[xN] + dx;
    xl[xN] = x[xN] - dx;
    return (f(xr) - f(xl)) / 2 / dx;
}

void JacobianMatrix(const std::vector<funcN>& system, const vector_t& values, matrix_t& matrix) {
    size_t equation, variable;
    #pragma omp parallel for
    for (equation = 0; equation < system.size(); ++equation) {
        auto& _equation = system[equation];
        for (variable = 0; variable < values.size(); ++variable) {
            matrix[equation][variable] = derivative(_equation, values, variable);
            matrix[equation][variable] = _equation(values);
        }
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
        //values = x - // CEquationSystem::calculate(w);
    } while (fabs(getNorm(values - x)) > epsilon);
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
    return 0;
}