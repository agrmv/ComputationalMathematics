//
// Created by agrmv on 02.03.18.
//
#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <iterator>

using matrix_t = std::vector<std::vector<double>>;
using vec = std::vector<double>;

template<class T>
class Matrix {
public:
    matrix_t matrix;
    Matrix() = default;
    Matrix(std::initializer_list<std::vector<T>> l) : matrix(l) {}

    void swapRows() {
        auto n = matrix.size();
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
    void matrixPrint() {
        for (auto &i : matrix) {
            for (auto &j : i) {
                std::cout << std::setw(6) << j;
            }
            std::cout << std::endl;
        }
    }
    void resultPrint(vec& vec1) {
        for (auto &i : vec1) {
            std::cout << i << "  ";
        }
        std::cout << std::endl;
    }
};

template <class T>
class Gauss : public Matrix<T> {
public:
    vec result;
    Gauss() = default;
    Gauss(std::initializer_list<std::vector<T>> l1) : Matrix<T>(l1) {}
    vec& calculate(matrix_t& matrix) {
        auto n = matrix.size();
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
};

template<class T>
class Seidel : public Matrix<T> {
public:
    vec variables;
    double eps;
    Seidel() = default;
    Seidel(std::initializer_list<std::vector<T>> l1) : Matrix<T>(l1) {}
    Seidel(std::initializer_list<T> l) : variables(l) {}
    vec& calculate(matrix_t& matrix) {
        auto n = matrix.size();
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


    vec& calculateIter(matrix_t& matrix) {
        vec var2(matrix.size());
        auto n = matrix.size();
        size_t flag=0, count=0;
        std::cout << "Iter" << std::setw(10);
        for(size_t  i = 0 ; i < n; ++i) {
            std::cout << "x" << i << std::setw(18);
        }
        std::cout<<"\n----------------------------------------------------------------------";
        do {
            std::cout << "\n" << count + 1 << "." << std::setw(16);
            for (size_t i = 0; i < n; ++i) {
                var2[i] = matrix[i][n];
                for (size_t j = 0; j < n; ++j) {
                    if (j != i) {
                        var2[i] -= matrix[i][j] * variables[j];
                    }
                }
                var2[i] /= matrix[i][i];
                if (fabs(var2[i] - variables[i]) <= eps) {
                    flag++;
                }
                std::cout << var2[i] << std::setw(18);
            }
            variables = var2;
            std::cout<<"\n";
            count++;
        } while(flag < n);

        return variables;
    }
};

