#include "Interpolation.h"
#include "Grid.h"

int main(int argc, char** argv) {
    //std::initializer_list<double> _x = {-1.0, -0.6, -0.3, 0.3, 1.0};
    //std::initializer_list<double> _y = { 0.5,  1.0,  0.4, 0.1, 0.5};
    std::initializer_list<double> _x = {-2, -1, 0, 1, 2};
    std::initializer_list<double> _y = { -8, -1, 0, 1, 8};
    std::vector<std::pair<double, double>> points;
    std::unique_ptr<Grid<double>> glut(new Grid<double>(_x, _y, 2));
    std::unique_ptr<Interpolation<double>> interpolation(new Interpolation<double>(_x, _y));
    std::cout << "1 - Newton\n2 - Lagrange\n3 - Aitken\n4 - Spline\n";
    uint16_t func;
    std::cin >> func;
    double x = 1.2273;
    //std::cout << "Enter pointsX: \n";
    //std::cin >> pointsX;
    switch(func) {
        case 1:
            std::cout << "Result: " << interpolation->newtonInterpolation(x) << std::endl;
            glut->initGlut(argc, argv, func);
            break;
        case 2:
            std::cout << "Result: " << interpolation->lagrangeInterpolation(x) << std::endl;
            glut->initGlut(argc, argv, func);
            break;
        case 3:
            std::cout << "Result: " << interpolation->aitkenInterpolation(x) << std::endl;
            glut->initGlut(argc, argv, func);
            break;
        case 4:
            std::cout << "Result: " << interpolation->splineInterpolation(x) << std::endl;
            glut->initGlut(argc, argv, func);
            break;
        default:break;
    }
    return 0;
}