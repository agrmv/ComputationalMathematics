#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include <iterator>

using vector_t = std::vector<double>;
static const int16_t xMin = -2;
static const int16_t xMax =  2;
static const int16_t yMin = -2;
static const int16_t yMax =  2;
static const double step  =  0.5;

std::ostream& operator<< (std::ostream& os, const vector_t& v) {
    for (auto &i: v) {
        os << i  << " ";
    }
    return os;
}

void drawStringBitmap(void *font, const char* string) {
    while (*string) {
        glutBitmapCharacter(font, *string++);
    }
}

double q(const vector_t &vec, double x, size_t n) {
    if (n >= vec.size()) {
        return 1;
    }
    double result = 1;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != n) {
            result *= (x - vec[i]) / (vec[n] - vec[i]);
        }
    }
    return result;
}

double p(const vector_t &_x, const vector_t &y, double x, size_t n) {
    double res = 0;
    for (size_t i = 0; i <= n; ++i) {
        res += y[i] * q(_x, x, i);
    }
    return res;
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
    /*size_t n;
    std::cout << "Enter n: ";
    std::cin >> n;
    vector_t _x(n + 1), y(n + 1);
    for (size_t i = 0; i < n + 1; ++i) {
        std::cout << "Enter x" << i << ": ";
        std::cin >> _x[i];
        std::cout << "Enter y" << i << ": ";
        std::cin >> y[i];
    }*/
    vector_t _x{-1.0, -0.6, -0.3, 0.3, 1.0};
    vector_t  y{ 0.5,  1.0,  0.4, 0.1, 0.5};
    if (_x.size() != y.size()) {
		throw std::logic_error("Error vector's size");
	}
    size_t n = _x.size();

    glPointSize(8);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
        glColor3f(1.0,0.0,0.0);
        for (auto &i: _x) {
            glVertex2d(i, p(_x, y, i, n));
        }
    glEnd();

    glBegin(GL_LINE_STRIP);
        glColor3f(1.0,0.0,0.0);
        auto leftX  = *std::min_element(_x.begin(), _x.end());
        auto rightX = *std::max_element(_x.begin(), _x.end());
        for (double i = leftX; i < rightX; i += 0.001) {
            glVertex2d(i, p(_x, y, i, n));
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