//
// Created by agrmv on 02.03.18.
//
#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <memory>

using vector_t = std::vector<double>;
using matrix_t = std::vector<vector_t>;

class NonlinearEquations {
private:
    double eps{};
public:
    vector_t variables;
    matrix_t matrix;
    vector_t result;
    NonlinearEquations() = default;
    NonlinearEquations(std::initializer_list<std::vector<double>> &m, const size_t& n) : matrix(m) {
        matrix.resize(n + 1);
    }
    NonlinearEquations(matrix_t &m, const size_t& n) : matrix(std::move(m)) {
        matrix.resize(n + 1);
    }
    NonlinearEquations(std::initializer_list<std::vector<double>> &m, std::initializer_list<double> l, const size_t& n) : matrix(m), variables(l) {
        matrix.resize(n + 1);
        variables.resize(n);
    }
    vector_t calculateGauss(matrix_t &matrix);
    vector_t calculateSeidel(matrix_t& matrix, const double& eps = 1e-5);
    void swapRows();
    double getNorm(vector_t&& vector);
    friend std::ostream &operator<<(std::ostream &os, const vector_t &v);
    friend std::istream &operator>>(std::istream &is, vector_t &v);
    friend std::ostream &operator<<(std::ostream& os, const matrix_t &m);
    friend vector_t operator-(const vector_t& vec1, const vector_t& vec2);
};

vector_t NonlinearEquations::calculateGauss(matrix_t &matrix) {
    result.resize(matrix.size(), 0);
    size_t n = matrix.size();
    for (size_t k = 0; k < n; ++k) {
        swapRows();
        for (size_t i = k + 1; i < n; ++i) {
            double c = -matrix[i][k] / matrix[k][k];
            for (size_t j = k; j < n + 1; ++j) {
                if (k == j) {
                    matrix[i][j] = 0;
                } else {
                    matrix[i][j] += c * matrix[k][j];
                }
            }
        }
    }
    for (size_t i = (n - 1); i < n; --i) {
        result[i] = matrix[i][n] / matrix[i][i];
        for (size_t k = i - 1; k < i; --k) {
            matrix[k][n] -= matrix[k][i] * result[i];
        }
    }
    return result;
}

vector_t NonlinearEquations::calculateSeidel(matrix_t &matrix, const double& eps) {
    size_t n = matrix.size();
    size_t flag=0, count=0;
    std::cout << "Iter" << std::setw(10);
    for(size_t  i = 0 ; i < n; ++i) {
        std::cout << "x" << i << std::setw(18);
    }
    std::cout<<"\n----------------------------------------------------------------------";
    do {
        std::cout << "\n" << count + 1 << "." << std::setw(16);
        for (size_t i = 0; i < n; ++i) {
            auto y = variables[i];
            variables[i] = matrix[i][n];
            for (size_t j = 0; j < n; ++j) {
                if (j != i) {
                    variables[i] -= matrix[i][j] * variables[j];
                }
            }
            variables[i] = variables[i] / matrix[i][i];
            if (fabs(variables[i] - y) <= eps) {
                flag++;
            }
            std::cout << variables[i] << std::setw(18);
        }
        std::cout<<"\n";
        count++;
    } while(flag < n);

    return variables;
}
void NonlinearEquations::swapRows() {
    size_t n = matrix.size();
    for (size_t k = 0; k < n; ++k) {
        auto maxEl = fabs(matrix[k][k]);
        auto maxRow = k;
        for (size_t i = k + 1; i < n; ++i) {
            if (fabs(matrix[i][k]) > maxEl) {
                maxEl = fabs(matrix[i][k]);
                maxRow = i;
            }
        }
        for (size_t i = k; i < n + 1; ++i) {
            std::swap(matrix[maxRow][i], matrix[k][i]);
        }
    }
}

double NonlinearEquations::getNorm(vector_t &&vector) {
    for (auto &i : vector) {
        if (i < 0) {
            i = -i;
        }
    }
    return *std::max_element(vector.cbegin(), vector.cend());
}

std::ostream &operator<<(std::ostream &os, const vector_t &v) {
    for (const auto &i: v) {
        os << i << " ";
    }
    return os;
}

std::istream &operator>>(std::istream &is, vector_t &v) {
    for (auto &i: v) {
        is >> i;
    }
    return is;
}

std::istream &operator>>(std::istream &is, matrix_t &m) {
    for (auto &i : m) {
        for (auto &j : i) {
            is >> j;
        }
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const matrix_t &m) {
    for (const  auto &i : m) {
        for (const auto &j : i) {
            os << std::setw(6) << j;
        }
        os << std::endl;
    }
}

vector_t operator-(const vector_t &vec1, const vector_t &vec2) {
    assert(vec1.size() == vec2.size());
    vector_t result(vec1.size(), 0);
    for (size_t iter = 0; iter != vec1.size(); ++iter) {
        result[iter] = vec1[iter] - vec2[iter];
    }
    return result;
}