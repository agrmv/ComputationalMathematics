#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

static const int SIZE = 3;
std::vector<std::vector<double>> matrix(SIZE, std::vector<double>(SIZE));
std::vector<double> result(matrix.size());

decltype(result) gauss(decltype(matrix)& matrix) {
    auto n = matrix.size();

    for (size_t k = 0; k < n; ++k) {
        auto maxEl = fabs(matrix[k][k]);
        int maxRow = k;
        for (size_t i = k + 1; i < n; ++i) {
            if (fabs(matrix[i][k]) > maxEl) {
                maxEl = fabs(matrix[i][k]);
                maxRow = i;
            }
        }
        for (size_t i = k; i < n + 1; ++i) {
            std::swap(matrix[maxRow][i], matrix[k][i]);
        }
        for (size_t i = k + 1; i < n; ++i) {
            double c = -matrix[i][k] / matrix[k][k];
            for (int j = k; j < n + 1; ++j) {
                if (k == j) {
                    matrix[i][j] = 0;
                } else {
                    matrix[i][j] += c * matrix[k][j];
                }
            }
        }
    }
    for (int i = n - 1; i >= 0; --i) {
        result[i] = matrix[i][n] / matrix[i][i];
        for (int k = i - 1; k >= 0; --k) {
            matrix[k][n] -= matrix[k][i] * result[i];
        }
    }
    return result;
}

void matrixPrint(decltype(matrix)& matrix) {
    for (auto &i : matrix) {
        for (auto &j : i) {
            std::cout << std::setw(SIZE) << j;
        }
        std::cout << std::endl;
    }

}

void resultPrint(decltype(result)& matrix) {
    for (auto &i : matrix) {
            std::cout << std::setw(SIZE) << i;
        }
    std::cout << std::endl;
}

int main() {
    matrix = {{1, 2 , 3, 1}, {4, 5, 6, 1}, {1, 0, 1, 1}};
    std::cout <<"Initial matrix: \n";
    matrixPrint(matrix);
    result = gauss(matrix);
    std::cout << "Result: \n";
    resultPrint(result);
    return 0;
}