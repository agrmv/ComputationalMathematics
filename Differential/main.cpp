#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <GL/glut.h>
#include <sstream>

struct Point {double x, y;};
using vector_t = std::vector<Point>;

void drawStringBitmap(void *font, const char *string) {
    while (*string) {
        glutBitmapCharacter(font, *string++);
    }
}

void drawFunc(vector_t & solve) {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (auto &i : solve) {
        glVertex2d(i.x, i.y);
    }
    glEnd();

}

void drawGrid(const double& xMin, const double& xMax, const double& stepDraw = 0.5) {
    double yMin = xMin;
    double yMax = xMax;
    glColor3f(0.0, 0.0, 0.0);
    //OX
    glBegin(GL_LINES);
    glVertex2d(xMin, 0);
    glVertex2d(xMax, 0);
    for (double i = xMin; i <= xMax; i += stepDraw) {
        glVertex2d(-0.05, i);
        glVertex2d( 0.05, i);
    }
    glEnd();

    for (double i = xMin; i <= xMax; i += stepDraw) {
        glRasterPos2d(i, -0.1);
        std::ostringstream ch;
        ch << i;
        drawStringBitmap(GLUT_BITMAP_HELVETICA_18, ch.str().c_str());
    }
    //OY
    glBegin(GL_LINES);
    glVertex2d(0, yMin);
    glVertex2d(0, yMax);
    for (double i = yMin; i <= yMax; i += stepDraw) {
        glVertex2d(i, -0.05);
        glVertex2d(i,  0.05);
    }
    glEnd();

    for (double i = yMin; i <= yMax; i += stepDraw) {
        if (i == 0) continue;
        glRasterPos2d(0.1, i);
        std::ostringstream ch;
        ch << i;
        drawStringBitmap(GLUT_BITMAP_HELVETICA_18, ch.str().c_str());
    }
}

void display(vector_t & solve, const double& a = -1, const double& b = 1) {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid(a, b);
    drawFunc(solve);
    glutSwapBuffers();
    glFlush();
}

vector_t rungeKutt4Method(const std::function<double(const double&, const double&)> &f, vector_t& solve, const double& a, const double& b, const double& h, const double& y0) {
    size_t n = (b - a) / h + 1;
    solve = vector_t(n);
    solve[0].x = a;
    solve[0].y = y0; // начальные условия

    for (int i = 1; i < n; i++) {
        solve[i].x = solve[i - 1].x + h;

        double k1 = f(solve[i - 1].x, solve[i - 1].y);
        double k2 = f(solve[i - 1].x + h / 2, solve[i - 1].y + h * k1 / 2);
        double k3 = f(solve[i - 1].x + h / 2, solve[i - 1].y + h * k2 / 2);
        double k4 = f(solve[i - 1].x + h, solve[i - 1].y + h * k3);

        solve[i].y = solve[i - 1].y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }

    return solve;
}

int main(int argc, char **argv) {
    double a, b, h, y0;
    std::cout << "Enter a: ";
    std::cin >> a;
    std::cout << "Enter b: ";
    std::cin >> b;
    std::cout << "Enter h: ";
    std::cin >> h;

    std::cout << "Enter y0: ";
    std::cin >> y0;
    std::vector<Point> solve;
    auto f = [](const double& x, const double& y) {
        return 3 * x * x * y;
    };
    solve = rungeKutt4Method(f, solve, a, b, h, y0);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(500, 500);
    glutCreateWindow("DiFF");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(a - 0.2, b + 0.2, a - 0.2, b + 0.2);
    display(solve);
    glutMainLoop();
    return 0;
}