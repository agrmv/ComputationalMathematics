#pragma once

#include <iomanip>
#include "Interpolation.h"

class Glut :public Interpolation<double> {
private:
    double step;
    double xMin;
    double xMax;
    double yMin;
    double yMax;
public:
    Glut() : step(0.5) {
        xMin = (*std::min_element(x.begin(), x.end()) < *std::min_element(y.begin(), y.end())) ? *std::min_element(x.begin(), x.end()) : *std::min_element(y.begin(), y.end());
        xMax = (*std::max_element(x.begin(), x.end()) > *std::max_element(y.begin(), y.end())) ? *std::max_element(x.begin(), x.end()) : *std::max_element(y.begin(), y.end());
        yMin = (*std::min_element(y.begin(), y.end()) < *std::min_element(x.begin(), x.end())) ? *std::min_element(y.begin(), y.end()) : *std::min_element(x.begin(), x.end());
        yMax = (*std::max_element(y.begin(), y.end()) > *std::max_element(x.begin(), x.end())) ? *std::max_element(y.begin(), y.end()) : *std::max_element(x.begin(), x.end());
    }

    Glut(std::initializer_list<double> _x, std::initializer_list<double> _y, const double& step) : Interpolation(_x, _y), step(step) {
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
                glFlush();
                break;
            case 2:
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINE_STRIP);
                for (auto i = xMin; i < xMax; i += 0.001) {
                    glVertex2d(i, lagrangeInterpolation(i));
                }
                glEnd();
                glFlush();
                break;
            case 3:
                glColor3f(1.0, 0.0, 0.0);
                glBegin(GL_LINE_STRIP);
                for (auto i = xMin; i < xMax; i += 0.001) {
                    glVertex2d(i, aitkenInterpolation(i));
                }
                glEnd();
                glFlush();
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
            glVertex2d(0.05, i);
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
            glVertex2d(i, 0.05);
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

    void drawResultPoint() {
        glPointSize(10);
        glBegin(GL_POINTS);
        glColor3f(0.0, 1.0, 0.0);
        //TODO
        glEnd();
    }

    void display(const uint16_t& func) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawGrid();
        drawFunc(func);
        drawResultPoint();
        glutSwapBuffers();
        glFlush();
    }

    void initGlut(int argc, char **argv, const uint16_t& func) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(800, 800);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Interpolation");
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(xMin - 0.2, xMax + 0.2, yMin - 0.2, yMax + 0.2);
        display(func);
        glutMainLoop();
    }
};