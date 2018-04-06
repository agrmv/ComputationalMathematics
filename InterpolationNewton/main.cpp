#include <iostream>
#include <cmath>
#include <vector>
#include <future>
#include <GL/glut.h>
#include <sstream>
#include <algorithm>

using vector_t = std::vector<double>;
static const int16_t xMin = -2;
static const int16_t xMax =  2;
static const int16_t yMin = -2;
static const int16_t yMax =  2;
static const double step  =  0.5;

std::istream& operator>>(std::istream& is, vector_t& v) {
    for(auto &i: v) {
        is >> i;
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const vector_t& v) {
    for(const auto &i: v) {
        os << i;
    }
    return os;
}

void drawStringBitmap(void *font, const char* string) {
    while (*string) {
        glutBitmapCharacter(font, *string++);
    }
}
//разделенная разность
double f(const int32_t& i, const size_t& k, const vector_t &x, const vector_t &y) {
    if (k > 1) {
        return (f(i + 1, k - 1, x, y) - f(i, k - 1, x, y)) / (x[i + k] - x[i]);
    }
    return k == 0 ? y[i] : (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
}

double interpolation(const double& _x, const vector_t &x, const vector_t &y, const size_t& n) {
    double sum = 0;
    double w = 1;
    for (size_t i = 0; i < n; i++) {
        sum += w * f(0, i, x, y);
        w *= (_x - x[i]);
    }
    return sum;
}

void drawGrid() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    //OX
    glBegin(GL_LINES);
    glVertex2d(xMin, 0);
    glVertex2d(xMax, 0);
    for (double i = xMin; i <= xMax; i += step) {
        glVertex2d(-0.05, i);
        glVertex2d( 0.05, i);
    }
    glEnd();

    for (double i = xMin; i <= xMax; i += step) {
        glRasterPos2d(i, -0.2);
        std::ostringstream ch;
        ch << i;
        drawStringBitmap(GLUT_BITMAP_HELVETICA_18, ch.str().c_str());
    }
    //OY
    glBegin(GL_LINES);
    glVertex2d(0, yMin);
    glVertex2d(0, yMax);
    for (double i = yMin; i <= yMax; i += step) {
        glVertex2d(i, -0.05);
        glVertex2d(i,  0.05);
    }
    glEnd();

    for (double i = yMin; i <= yMax; i += step) {
        if (i == 0) continue;
        glRasterPos2d(0.1, i);
        std::ostringstream ch;
        ch << i;
        drawStringBitmap(GLUT_BITMAP_HELVETICA_18, ch.str().c_str());
    }
}

void display() {
    /*vector_t  x(5);
    vector_t  y(5);
    std::cout << "Enter vector x: \n";
    std::cin >> x;
    std::cout << "Enter vector y: \n";
    std::cin >> y;*/
    vector_t  x{-1.0, -0.6, -0.3, 0.3, 1.0};
    vector_t  y{ 0.5,  1.0,  0.4, 0.1, 0.5};
    if (x.size() != y.size()) {
        throw std::logic_error("Error vector's size");
    }
    size_t n = x.size();

    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    auto leftX  = *std::min_element(x.begin(), x.end());
    auto rightX = *std::max_element(x.begin(), x.end());
    for (auto i = leftX; i < rightX; i += 0.001) {
        glVertex2d(i, interpolation(i, x, y, n));
    }
    glEnd();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0,0);
    glutCreateWindow("LAGRANGE");
    glClearColor(1.0 , 1.0 , 1.0 , 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xMin - 0.2, xMax + 0.2, yMin - 0.2, yMax + 0.2);
    drawGrid();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}