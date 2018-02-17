//
// Created by agrmv on 17.02.18.
//
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class CEquationSystem {
    typedef vector<vector<double> > matrix_t;
    typedef matrix_t::iterator matrix_row;
    typedef matrix_t::const_iterator matrix_crow;
    class methodGauss {
        static void row_swap(matrix_row start, matrix_row end, uint16_t column) {
            matrix_row max(start);
            for (; start != --end;)
                if (fabs(max->operator[](column)) < fabs(end->operator[](column)))
                    max = end;
            if (max != start)
                iter_swap(max, start);
        }

        static vector<double> calculate(const matrix_t &mat) {
            int free_id = mat.size(), left = free_id - 1;
            vector<double> variables(free_id);
            matrix_crow row = mat.end() - 1;
            for (; left >= 0; --left, --row) {
                variables[left] = (*row)[free_id];
                for (int current = free_id - 1; current > left; --current)
                    variables[left] -= (*row)[current] * variables[current];
                variables[left] /= (*row)[left];
            }
            return variables;
        }
    public:
        static vector<double> solving(matrix_t &mat) /*throw(logic_error)*/ {
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

    matrix_t matrix;

public:

    void loadData(unsigned n) {
        matrix.resize(n);
        for (unsigned i = 0; i < n; ++i) {
            cout << "Enter " << n << " coefficients for unknowns and 1 free variable: ";
            matrix[i].resize(n + 1);
            for (unsigned j = 0; j < n + 1; ++j)
                cin >> matrix[i][j];
        }
    }

    void calculate() {
        try {
            vector<double> unknowns = methodGauss::solving(matrix);
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
    system.calculate();
    return 0;
}