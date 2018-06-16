#include "Interpolation.h"
#include "Grid.h"

int main(int argc, char** argv) {
    initlist_t<double> pointsX = { -2, -1, 0, 1, 2};
    initlist_t<double> pointsY = { -8, -1, 0, 1, 8};
    double serifOffset = 2.0;
    double x = 0;
    std::cout << "1 - Newton\n2 - Lagrange\n3 - Aitken\n4 - Spline\n5 - Trigonometric" << std::endl;
    uint16_t func;
    std::cin >> func;
    std::cout << "Enter x:" << std::endl;
    std::cin >> x;
    std::unique_ptr<Grid<double>> glut(new Grid<double>(pointsX, pointsY, x, serifOffset));
    std::unique_ptr<Interpolation<double>> interpolation(new Interpolation<double>(pointsX, pointsY));
    std::cout << interpolation->pointsX << std::endl;
    std::cout << interpolation->pointsY << std::endl;
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
        case 5:
            std::cout << "Result: " << interpolation->trigonometricInterpolatation(x) << std::endl;
            glut->initGlut(argc, argv, func);
            break;
        default:break;
    }
    return 0;
}