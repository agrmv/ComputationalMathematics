#include "header.h"

int main() {
    uint16_t temp;
    uint16_t size;
    //std::cout.precision(4);
    std::cout.setf(std::ios::fixed);
    std::cout << "1 - Seidel\n2 - Gauss" << std::endl;
    std::cin >> temp;
    switch (temp) {
        case 1: {
            std::cout << "\nEnter the elements of the augmented matrix row-wise:\n";
            std::cin >> size;
            //std::cout << "\nEnter the accuracy upto which you want the solution:\n";
            //std::cin >> seidel->eps;
            std::initializer_list<std::vector<double>> m = {{ 5,   1,   2,   4},
                                                            {-3,  10,   2,  22},
                                                            { 2,  -1,  -5,  -7}};
            std::initializer_list<double> var = {0, 0, 0};
            std::unique_ptr<NonlinearEquations> seidel(new NonlinearEquations(m, var, size));
            std::cout << "Initial matrix: \n";
            std::cout << seidel->matrix;
            seidel->calculateSeidel(seidel->matrix);
            std::cout << "\n The solution is as follows:\n";
            std::cout << seidel->variables;
            break;
        }
        case 2: {
            std::cout << "\nEnter the elements of the augmented matrix row-wise:\n";
            std::cin >> size;
            std::initializer_list<std::vector<double>> m = {{ 5,   1,   2,   4},
                                                            {-3,  10,   2,  22},
                                                            { 2,  -1,  -5,  -7}};
            std::unique_ptr<NonlinearEquations> gauss(new NonlinearEquations(m, size));
            std::cout << "Initial matrix: \n";
            std::cout << gauss->matrix;
            gauss->calculateGauss(gauss->matrix);
            std::cout << "Result: \n";
            std::cout << gauss->result;
            break;
        }
        default:break;
    }
    return 0;
}
