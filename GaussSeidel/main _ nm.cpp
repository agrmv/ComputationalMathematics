#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

namespace solvingEquationSystem {
    typedef vector<double> vector_t;
    typedef vector<vector_t > matrix_t;
    typedef matrix_t::iterator matrix_row;
    typedef matrix_t::const_iterator matrix_crow;

    namespace methodGauss {
        void row_swap(matrix_row start, matrix_row end, uint16_t column) {
            matrix_row max(start);
            for (; start != --end;)
                if (fabs(max->operator[](column)) < fabs(end->operator[](column)))
                    max = end;
            if (max != start)
                iter_swap(max, start);
        }

        vector_t calculate(matrix_t &mat) {
            size_t free_id = mat.size(), left = free_id - 1;
            vector_t variables(free_id);
            matrix_row row = mat.end() - 1;
            for (; left < free_id; --left, --row) {
                variables[left] = (*row)[free_id] / (*row)[left];
                for (size_t current = left; current > 0; --current)
                    (*(row - current))[free_id] -= (*(row - current))[left] * variables[left];
            }
            return variables;
        }

        /*static vector_t calculate(const matrix_t &mat) {
            size_t free_id = mat.size(), left = free_id - 1;
            vector_t variables(free_id);
            matrix_crow row = mat.end() - 1;
            for (; left < free_id; --left, --row) {
                variables[left] = (*row)[free_id];
                for (size_t current = free_id - 1; current > left; --current)
                    variables[left] -= (*row)[current] * variables[current];
                variables[left] /= (*row)[left];
            }
            return variables;
        }*/

        vector_t solving(matrix_t &mat) /*throw(logic_error)*/ {
            matrix_row start = mat.begin(), end = mat.end();
            uint16_t column = 0;
            for (; start != end - 1; ++start, ++column) {
                row_swap(start, end, column);
                if (!(*start)[column])
                    throw logic_error("На главной диагонали 0!");
                for (matrix_row iter = start + 1; iter != end; ++iter) {
                    double coeff = (*iter)[column] / (*start)[column];
                    for (uint16_t item = column; item < start->size(); ++item)
                        (*iter)[item] -= (*start)[item] * coeff;
                }
            }
            return calculate(mat);
        }
    };

    namespace methodIteration {
        namespace peripheral {
            double ln(double value) {
                return log(value) / log(M_E);
            }

            double getNorm(vector_t vector) {
                for (double &item : vector)
                    if (__glibc_unlikely(item < 0))
                        item = -item;
                return *max_element(vector.cbegin(), vector.cend());
            }

            double getNorm(const matrix_t &matrix) {
                vector_t lines(matrix.size(), 0);
                for (size_t row = 0, end = matrix.size(); row != end; ++row)
                    for (size_t column = 0; column != end; ++column)
                        lines[row] += abs(matrix[row][column]);
                return *max_element(lines.cbegin(), lines.cend());
            }

            size_t stepCount(double epsilon, const matrix_t &matrix, const vector_t b) {
                double normC = getNorm(matrix);
                double normB = getNorm(b);
                return size_t(ln(epsilon * (1 - normC) / normB) / ln(normC) + 1);
            }

            vector_t splitMatrix(matrix_t &matrix) {
                vector_t b(matrix.size());
                for (size_t row = 0, end = matrix.size(); row != end; ++row) {
                    for (size_t column = 0; column < matrix[row].size(); ++column)
                        if (__glibc_likely(column != row))
                            matrix[row][column] /= matrix[row][row];
                    matrix[row][row] = 0;
                    b[row] = matrix[row][matrix[row].size() - 1];
                }
                return b;
            }

            vector_t operator-(const vector_t &v1, const vector_t &v2) {
                if (v1.size() != v2.size())
                    throw logic_error("v1.size() != v2.size()");
                vector_t result(v1.size());
                for (size_t iter = 0; iter != v1.size(); ++iter)
                    result[iter] = v1[iter] - v2[iter];
                return result;
            }

            double operator*(const vector_t &v1, const vector_t &v2) {
                if (v1.size() < v2.size())
                    throw logic_error("v1.size() < v2.size()");
                double result = 0;
                for (size_t iter = 0; iter < v1.size(); ++iter)
                    result += v1[iter] * v2[iter];
                return result;
            }

            vector_t operator*(const matrix_t &m, const vector_t &v) {
                if (m[0].size() < v.size())
                    throw logic_error("m[0].size() < v.size()");
                vector_t result(m.size(), 0);
                size_t row = 0, end = v.size();
                for (; row != end; ++row)
                    result[row] = m[row] * v;
                return result;
            }

        };

        vector_t solvingSimple(matrix_t &matrix, double epsilon) {
            using namespace peripheral;
            vector_t b = splitMatrix(matrix), unknowns(b);
            size_t max = stepCount(epsilon, matrix, b);
            for (size_t step = 1; step < max; ++step) {
                vector_t tmp(unknowns);
                unknowns = b - matrix * unknowns;
                if (getNorm(unknowns - tmp) <= epsilon)
                    break;
            }
            return unknowns;
        }

        vector_t solvingSeidel(matrix_t &matrix, double epsilon) {
            using namespace peripheral;
            vector_t b = splitMatrix(matrix), unknowns(b);
            size_t max = stepCount(epsilon, matrix, b);
            for (size_t step = 1; step < max; ++step) {
                vector_t tmp(unknowns);
                for (size_t row = 0; row < matrix.size(); ++row)
                    unknowns[row] = b[row] - matrix[row] * unknowns;
                if (getNorm(unknowns - tmp) <= epsilon)
                    break;
            }
            return unknowns;
        };
    };
};

class CEquationSystem {
    using vector_t = solvingEquationSystem::vector_t;
    using matrix_t = solvingEquationSystem::matrix_t;

    matrix_t matrix;

public:
    enum class eType {
        Gauss,
        SimpleIter,
        SeidelIter
    };

    void loadData(unsigned n) {
        matrix.resize(n);
        for (unsigned i = 0; i < n; ++i) {
            cout << "Enter " << n << " coefficients for unknowns and 1 free variable: ";
            matrix[i].resize(n + 1);
            for (unsigned j = 0; j < n + 1; ++j)
                cin >> matrix[i][j];
        }
    }

    void calculate(eType type) {
        try {
            using namespace solvingEquationSystem;
            vector_t unknowns;
            if (type == eType::Gauss)
                unknowns = methodGauss::solving(matrix);
            else {
                double epsilon;
                cout << "Enter epsilon: "; cin >> epsilon;
                if (type == eType::SimpleIter)
                    unknowns = methodIteration::solvingSimple(matrix, epsilon);
                else
                    unknowns = methodIteration::solvingSeidel(matrix, epsilon);
            }
            for (unsigned i = 0; i < unknowns.size(); ++i)  //for (auto unknown : unknowns)
                cout << "X" << i + 1 << " = " << unknowns[i] << endl;
        } catch (const std::logic_error &ex) {
            cerr << "Решение не возможно!\nОписание: " << ex.what();
        }
    }
};

int main() {
    CEquationSystem system;
    unsigned count;
    cout << "Enter count equations: ";
    cin >> count;
    system.loadData(count);
    system.calculate(CEquationSystem::eType::SimpleIter);
    return 0;
}
