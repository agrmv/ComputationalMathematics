#pragma once

#include <iomanip>
#include "Interpolation.h"

template <class T>
class Grid : protected Interpolation<double> {
private:
    T xMin;
    T xMax;
    T yMin;
    T yMax;
    double serifOffset;
    double resulPoint;
    double h{0.01};
    pair_t minmax_x = std::minmax_element(pointsX.begin(), pointsX.end());
    pair_t minmax_y = std::minmax_element(pointsY.begin(), pointsY.end());
    Grid() : serifOffset(0.5), resulPoint(0) {
        xMin = (*minmax_x.first < *minmax_y.first ? *minmax_x.first : *minmax_y.first);
        yMin = (*minmax_y.first < *minmax_x.first ? *minmax_y.first : *minmax_x.first);

        if (*minmax_x.second > *minmax_y.second && *minmax_x.second > fabs(xMin)) {
            xMax = *minmax_x.second;
        } else if(*minmax_y.second > *minmax_x.second && *minmax_y.second > fabs(xMin)) {
            xMax = *minmax_y.second;
        } else {
            xMax = fabs(xMin);
        }

        if (*minmax_y.second > *minmax_x.second && *minmax_y.second > fabs(yMin)) {
            yMax = *minmax_y.second;
        } else if(*minmax_x.second > *minmax_y.second && *minmax_x.second > fabs(yMin)) {
            yMax = *minmax_x.second;
        } else {
            yMax = fabs(yMin);
        }

    }

    void drawStringBitmap(const double& x, const double& y, const uint8_t& axis) {
        glRasterPos2d(x, y);
        std::stringstream sout;
        sout << ((axis) ? x : y);
        for (char draw : sout.str()) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, draw);
        }
    }

    void drawFunc(const uint16_t& func) {
        switch(func) {
            case 1:
                glPointSize(8);
                glBegin(GL_POINTS);
                    glColor3f(1.0, 0.0, 1.0);
                    glVertex2d(resulPoint, newtonInterpolation(resulPoint));
                    glColor3f(0.0, 0.0, 1.0);
                    for (const double &i: pointsX) {
                        glVertex2d(i, newtonInterpolation(i));
                     }
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glColor3f(1.0, 0.0, 0.0);
                    for (double i = *minmax_x.first; i <= *minmax_x.second; i += h) {
                        glVertex2d(i, newtonInterpolation(i));
                    }
                glEnd();
                break;
            case 2:
                glPointSize(8);
                glBegin(GL_POINTS);
                    glColor3f(1.0, 0.0, 1.0);
                    glVertex2d(resulPoint, lagrangeInterpolation(resulPoint));
                    glColor3f(0.0, 0.0, 1.0);
                    for (const double &i: pointsX) {
                        glVertex2d(i, lagrangeInterpolation(i));
                    }
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glColor3f(1.0, 0.0, 0.0);
                    for (double i = *minmax_x.first; i <= *minmax_x.second; i += h) {
                        glVertex2d(i, lagrangeInterpolation(i));
                    }
                glEnd();
                break;
            case 3:
                glPointSize(8);
                glBegin(GL_POINTS);
                    glColor3f(1.0, 0.0, 1.0);
                    glVertex2d(resulPoint, aitkenInterpolation(resulPoint));
                    glColor3f(0.0, 0.0, 1.0);
                    for (const double &i: pointsX) {
                        glVertex2d(i, aitkenInterpolation(i));
                    }
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glColor3f(1.0, 0.0, 0.0);
                    for (double i = *minmax_x.first; i <= *minmax_x.second; i += h) {
                        glVertex2d(i, aitkenInterpolation(i));
                    }
                glEnd();
                break;
            case 4:
                glPointSize(8);
                glBegin(GL_POINTS);
                    glColor3f(1.0, 0.0, 1.0);
                    glVertex2d(resulPoint, splineInterpolation(resulPoint));
                    glColor3f(0.0, 0.0, 1.0);
                    for (const double &i: pointsX) {
                        glVertex2d(i, splineInterpolation(i));
                    }
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glColor3f(1.0, 0.0, 0.0);
                    for (double i = *minmax_x.first; i <= *minmax_x.second; i += h) {
                        glVertex2d(i, splineInterpolation(i));
                    }
                glEnd();
                break;
            case 5:
                glPointSize(8);
                glBegin(GL_POINTS);
                    glColor3f(1.0, 0.0, 1.0);
                    glVertex2d(resulPoint, trigonometricInterpolatation(resulPoint));
                    glColor3f(0.0, 0.0, 1.0);
                    for (const double &i: pointsX) {
                        glVertex2d(i, trigonometricInterpolatation(i));
                    }
                glEnd();
                glBegin(GL_LINE_STRIP);
                    glColor3f(1.0, 0.0, 0.0);
                    for (double i = *minmax_x.first; i <= *minmax_x.second; i += h) {
                        glVertex2d(i, trigonometricInterpolatation(i));
                    }
                glEnd();
                break;
            default:break;
        }
    }

    void drawGrid() {
        auto minmax_x = std::minmax_element(pointsX.begin(), pointsX.end());
        glColor3f(0.0, 0.0, 0.0);
        uint8_t axisX = 1, axisY = 0;
        //AXIS X
        glBegin(GL_LINES);
        glVertex2d(xMin, 0);
        glVertex2d(xMax, 0);
        for (double i = xMin; i <= xMax; i += serifOffset) {
            glVertex2d(-0.05, i);
            glVertex2d( 0.05, i);
        }
        glEnd();

        for (double i = xMin; i <= xMax; i += serifOffset) {
            drawStringBitmap(i, -0.25, axisX);
        }

        //AXIS Y
        glBegin(GL_LINES);
        glVertex2d(0, yMin);
        glVertex2d(0, yMax);
        for (double i = yMin; i <= yMax; i += serifOffset) {
            glVertex2d(i, -0.05);
            glVertex2d(i,  0.05);
        }
        glEnd();

        for (double i = yMin; i <= yMax; i += serifOffset) {
            if (i == 0) continue;
            drawStringBitmap(0.1, i, axisY);
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
        glutInitDisplayMode(GLUT_RGB);
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

    Grid(const initlist_t<T>& _x, const initlist_t<T>& _y, const double& _drawPoint, const double& serifOffset = 0.5)
            : Interpolation(_x, _y), serifOffset(serifOffset), resulPoint(_drawPoint) {
        xMin = (*minmax_x.first < *minmax_y.first ? *minmax_x.first : *minmax_y.first);
        yMin = (*minmax_y.first < *minmax_x.first ? *minmax_y.first : *minmax_x.first);

        if (*minmax_x.second > *minmax_y.second && *minmax_x.second > fabs(xMin)) {
            xMax = *minmax_x.second;
        } else if(*minmax_y.second > *minmax_x.second && *minmax_y.second > fabs(xMin)) {
            xMax = *minmax_y.second;
        } else {
            xMax = fabs(xMin);
        }

        if (*minmax_y.second > *minmax_x.second && *minmax_y.second > fabs(yMin)) {
            yMax = *minmax_y.second;
        } else if(*minmax_x.second > *minmax_y.second && *minmax_x.second > fabs(yMin)) {
            yMax = *minmax_x.second;
        } else {
            yMax = fabs(yMin);
        }


    }
    Grid(const vector_t<T>& _x, const vector_t<T>& _y, const double& _drawPoint, const double& serifOffset = 0.5)
            : Interpolation(_x, _y), serifOffset(serifOffset), resulPoint(_drawPoint) {
        auto minmax_x = std::minmax_element(pointsX.begin(), pointsX.end());
        auto minmax_y = std::minmax_element(pointsY.begin(), pointsY.end());
        xMin = (*minmax_x.first < *minmax_y.first ? *minmax_x.first : *minmax_y.first);
        yMin = (*minmax_y.first < *minmax_x.first ? *minmax_y.first : *minmax_x.first);

        if (*minmax_x.second > *minmax_y.second && *minmax_x.second > fabs(xMin)) {
            xMax = *minmax_x.second;
        } else if(*minmax_y.second > *minmax_x.second && *minmax_y.second > fabs(xMin)) {
            xMax = *minmax_y.second;
        } else {
            xMax = fabs(xMin);
        }

        if (*minmax_y.second > *minmax_x.second && *minmax_y.second > fabs(yMin)) {
            yMax = *minmax_y.second;
        } else if(*minmax_x.second > *minmax_y.second && *minmax_x.second > fabs(yMin)) {
            yMax = *minmax_x.second;
        } else {
            yMax = fabs(yMin);
        }
    }
};