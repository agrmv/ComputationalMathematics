#include "Interpolation.h"
#include "Glut.h"

int main(int argc, char** argv) {
    /*size_t n;
    std::cout << "Enter n: ";
    std::cin >> n;
    vector_t<double> _x(n + 1), _y(n + 1);
    std::cout << "Enter x:\n";
    std::cin >> _x;
    std::cout << "Enter y:\n";
    std::cin >> _y;*/

    std::initializer_list<double> _x = {-1.0, -0.6, -0.3, 0.3, 1.0};
    std::initializer_list<double> _y = { 0.5,  1.0,  0.4, 0.1, 0.5};
    std::unique_ptr<Glut> glut(new Glut(_x, _y, 0.5));
    std::cout << "1 - Newton\n2 - Lagrange\n3 - Aitken\n";
    uint16_t func;
    std::cin >> func;
    glut->initGlut(argc, argv, func);
    return 0;
}