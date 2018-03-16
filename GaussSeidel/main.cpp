#include "header.h"

int main() {
    uint16_t temp;
    uint16_t size;
    std::cout.precision(4);
    //std::cout.setf(std::ios::fixed);
    std::cout << "1 - Seidel\n2 - Gauss" << std::endl;
    std::cin >> temp;
    switch (temp) {
        case 1: {
            std::unique_ptr<Seidel<double>> seidel(new Seidel<double>);
            std::cout << "\nEnter the elements of the augmented matrix row-wise:\n";
            std::cin >> size;
            std::cout << "\nEnter the accuracy upto which you want the solution:\n"; //0,00001
            std::cin >> seidel->eps;
            seidel->matrix.resize(size, vector_t(size + 1));
            seidel->variables.resize(size);
            seidel->matrix = {{5, 1,  2, 4},
                              {-3,  10, 2,  22},
                              {2,  -1,  -5, -7}};
            seidel->variables = {0, 0, 0};
            seidel->matrixPrint();
            seidel->swapRows();
            seidel->variables = seidel->calculate(seidel->matrix);
            //seidel->variables = seidel->calculateIter(seidel->matrix);
            std::cout << "\n The solution is as follows:\n";
            seidel->vectorPrint(seidel->variables);
            break;
        }
        case 2: {
            std::unique_ptr<Gauss<double>> gauss(new Gauss<double>);
            std::cout << "\nEnter the elements of the augmented matrix row-wise:\n";
            std::cin >> size;
            gauss->matrix.resize(size, vector_t(size + 1));
            gauss->result.resize(size);
            /*gauss->matrix = {{5, 1,  2, 4},
                             {-3,  10, 2,  22},
                             {2,  -1,  -5, -7}};*/
            gauss->matrix = {{-3,   2,  -4, -6},
                             { 5,  -1,   2,  3},
                             { 3,  -1,   1,  4}};
            std::cout << "Initial matrix: \n";
            gauss->matrixPrint();
            gauss->result = gauss->calculate(gauss->matrix);
            std::cout << "Result: \n";
            gauss->vectorPrint(gauss->result);
            break;
        }
        default:break;
    }
    return 0;
}
