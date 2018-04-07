#pragma once

#include <iomanip>
#include "Interpolation.h"

template <class T>
class Grid : protected Interpolation<double> {
private:
    double step;
    T xMin;
    T xMax;
    T yMin;
    T yMax;
    Grid() : step(0.5) {
        xMin = (*std::min_element(x.begin(), x.end()) < *std::min_element(y.begin(), y.end())) ? *std::min_element(x.begin(), x.end()) : *std::min_element(y.begin(), y.end());
        xMax = (*std::max_element(x.begin(), x.end()) > *std::max_element(y.begin(), y.end())) ? *std::max_element(x.begin(), x.end()) : *std::max_element(y.begin(), y.end());
        yMin = (*std::min_element(y.begin(), y.end()) < *std::min_element(x.begin(), x.end())) ? *std::min_element(y.begin(), y.end()) : *std::min_element(x.begin(), x.end());
        yMax = (*std::max_element(y.begin(), y.end()) > *std::max_element(x.begin(), x.end())) ? *std::max_element(y.begin(), y.end()) : *std::max_element(x.begin(), x.end());
    }

    void drawStringBitmap(void *font, const char *string) {
        while (*string) {
            glutBitmapCharacter(font, *string++);
        }
    }

    void drawFunc(const uint16_t& func) {
        switch(func) {
            case 1:
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINE_STRIP);
                    for (auto i = xMin; i < xMax; i += 0.001) {
                        glVertex2d(i, newtonInterpolation(i));
                    }
                glEnd();
                break;
            case 2:
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINE_STRIP);
                for (auto i = xMin; i < xMax; i += 0.001) {
                    glVertex2d(i, lagrangeInterpolation(i));
                }
                glEnd();
                break;
            case 3:
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINE_STRIP);
                for (auto i = xMin; i < xMax; i += 0.001) {
                    glVertex2d(i, aitkenInterpolation(i));
                }
                glEnd();
                break;
            default:break;
        }
    }

    void drawGrid() {
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
            glRasterPos2d(i, -0.1);
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

    void display(const uint16_t& func) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawGrid();
        drawFunc(func);
        glutSwapBuffers();
        glFlush();
    }

public:
    void initGlut(int argc, char **argv, const uint16_t& func) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(1000, 1000);
        glutInitWindowPosition(500, 500);
        glutCreateWindow("Interpolation");
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(xMin - 0.2, xMax + 0.2, yMin - 0.2, yMax + 0.2);
        display(func);
        glutMainLoop();
    }

    Grid(std::initializer_list<T> _x, std::initializer_list<T> _y, const double& step = 0.5) : Interpolation(_x, _y), step(step) {
        xMin = (*std::min_element(x.begin(), x.end()) < *std::min_element(y.begin(), y.end())) ? *std::min_element(x.begin(), x.end()) : *std::min_element(y.begin(), y.end());
        xMax = (*std::max_element(x.begin(), x.end()) > *std::max_element(y.begin(), y.end())) ? *std::max_element(x.begin(), x.end()) : *std::max_element(y.begin(), y.end());
        yMin = (*std::min_element(y.begin(), y.end()) < *std::min_element(x.begin(), x.end())) ? *std::min_element(y.begin(), y.end()) : *std::min_element(x.begin(), x.end());
        yMax = (*std::max_element(y.begin(), y.end()) > *std::max_element(x.begin(), x.end())) ? *std::max_element(y.begin(), y.end()) : *std::max_element(x.begin(), x.end());
    }
    Grid(const vector_t<T>& _x, const vector_t<T>& _y, const double& step = 0.5) : Interpolation(_x, _y), step(step) {
        xMin = (*std::min_element(x.begin(), x.end()) < *std::min_element(y.begin(), y.end())) ? *std::min_element(x.begin(), x.end()) : *std::min_element(y.begin(), y.end());
        xMax = (*std::max_element(x.begin(), x.end()) > *std::max_element(y.begin(), y.end())) ? *std::max_element(x.begin(), x.end()) : *std::max_element(y.begin(), y.end());
        yMin = (*std::min_element(y.begin(), y.end()) < *std::min_element(x.begin(), x.end())) ? *std::min_element(y.begin(), y.end()) : *std::min_element(x.begin(), x.end());
        yMax = (*std::max_element(y.begin(), y.end()) > *std::max_element(x.begin(), x.end())) ? *std::max_element(y.begin(), y.end()) : *std::max_element(x.begin(), x.end());
    }
};