#pragma once
#include <GL/glut.h>
#include <vector>
#include <utility>
#include <sstream>
#include <cassert>
#include <cmath>

using namespace std;

class CMyGlut {
    double y_coord, x_coord;
    double y_min, x_min;
    vector<pair<double, double> > points;
    vector<pair<double, double> > result;
    
    CMyGlut(int *argc, char **argv) {
        glutInit(argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutInitWindowPosition(500, 200);
        glutCreateWindow("data");
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glutDisplayFunc(display);
    }
    
    template <typename type>
    inline uint8_t getByte(type value, const size_t number = 0) {
        assert(sizeof(type) > number);
        return ((value >> (number * 8)) & 0xFF);
    }
    
    void rgbColor(uint32_t uRBG) {
        glColor3b(getByte(uRBG, 2), getByte(uRBG, 1), getByte(uRBG));
    }
    
    enum eStipple {
        ONE = 0x00FF, /*Factor:
        1: ____     _____     _____
        2: _________         ___________        __________*/
        TWO = 0x0C0F, /*Factor:
        1: ___  _  ___  _  ___  
        2: _______        _______        _______*/
        THREE = 0xAAAA, /*Factor:
        1: - - - - - - -
        2: _ _ _ _ _ _ _ _ _ _ _
        3: __  __  __  __ __ __
        4: ___   ___   ___   ___*/
    };
    
    static CMyGlut *pInstance;
    
    template<typename type>
    void printText(double x, double y, type val) {
        glRasterPos2d(x, y);
        std::stringstream sout;
        sout << val;   
        for (char draw : sout.str())
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, draw);
    }
    
    void getMiddle(double width, double step, double &left, double &right) {
        double middle = width / 2;
        double offset = middle - int(middle / step) * step;
        right = left = middle - offset;
        if (offset > __DBL_EPSILON__)
            left = right - step;
    }
    
    void drawgrid(double serifSize, double serifOffset) {
        glColor3f(0.0, 0.0, 0.0);
        
        glBegin(GL_LINES);
        glVertex2f(1, 1);//горизонталь
        glVertex2f(x_coord + 1, 1);
        glVertex2f(1, y_coord + 1);//вертикаль
        glVertex2f(1, 1);
        glEnd();
        //засечки по горизонтали
        double p, i;
        getMiddle(x_coord, serifOffset, p, i);
        for(; i <= x_coord; i += serifOffset, p -= serifOffset) {
            glBegin(GL_LINES);
            glVertex2f(i + 1, 1);
            glVertex2f(i + 1, 1 + serifSize);
            glVertex2f(p + 1, 1);
            glVertex2f(p + 1, 1 + serifSize);
            glEnd();
            
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0xAAAA);
            glBegin(GL_LINES);
            glVertex2f(i + 1, 1);
            glVertex2f(i + 1, y_coord + 1);
            glVertex2f(p + 1, 1);
            glVertex2f(p + 1, y_coord + 1);
            glEnd();
            glDisable(GL_LINE_STIPPLE);
            
            
            printText(i + 0.85, 0.65, x_min + i);
            printText(p + 0.85, 0.65, x_min + p);
        }
        
        serifSize *= abs(x_coord / y_coord);
        serifOffset /= abs(int(x_coord / y_coord) * 0.5 - 1);
        getMiddle(y_coord, serifOffset, p, i);
        //засечки по вертикали
        for(; i <= y_coord; i += serifOffset, p -= serifOffset) {
            glBegin(GL_LINES);
            glVertex2f(1, i + 1);
            glVertex2f(1 + serifSize , i + 1);
            glVertex2f(1, p + 1);
            glVertex2f(1 + serifSize, p + 1);
            glEnd();
            
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0xAAAA);
            glBegin(GL_LINES);
            glVertex2f(1, i + 1);
            glVertex2f(x_coord + 1, i + 1);
            glVertex2f(1, p + 1);
            glVertex2f(x_coord + 1, p + 1);
            glEnd();
            glDisable(GL_LINE_STIPPLE);
            
            printText(0.3, i + 1, y_min + i);
            printText(0.3, p + 1, y_min + p);
        }
    }
    
    void drawfunc() {
        glLineWidth(3);//glPointSize(3);
        glBegin(GL_LINES);
        glColor3f(0.8, 0.0, 0.8);
        for(int i = 0; i < points.size() - 1; ++i) {
            glVertex2f(points[i].first - x_min + 1, points[i].second - y_min + 1);
            glVertex2f(points[i + 1].first - x_min + 1, points[i + 1].second - y_min + 1);
        }
        glEnd();
        glLineWidth(1);
    }
    
    void drawresult() {
        glPointSize(4);
        glBegin(GL_POINTS);
        glColor3f(0, 0.8, 0.8);
        for(int i = 0; i < result.size(); ++i)
            glVertex2f(result[i].first - x_min + 1, result[i].second - y_min + 1);
        glEnd();
    }
    
    static void display() {
        if (pInstance) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            double offset = 1;//(pInstance->y_coord - pInstance->y_min) / 5 > 23 ? (pInstance->y_coord - pInstance->y_min) / 23 : 5;
            pInstance->drawgrid(0.1, offset);
            pInstance->drawfunc();
            pInstance->drawresult();

            glutSwapBuffers();
            glFlush();
        }
    }
public:
    static CMyGlut* init(int *argc, char **argv) {
        if (pInstance == nullptr)
            pInstance = new CMyGlut(argc, argv);
        return pInstance;
    }
    
    void displayFunc(const vector<pair<double, double> >& values, const vector<pair<double, double> >& result) {
        const pair<double, double> &max = values[values.size() - 1],
        &min = values[0];
        
        x_min = min.first - (min.first < 0 ? 5 + int(min.first) % 5 : int(min.first) % 5);
        y_min = min.second - (min.second < 0 ? 5 + int(min.second) % 5 : int(min.second) % 5);
        
        x_coord = max.first + 5 - abs(int(max.first) % 5) - x_min + 1;
        y_coord = max.second + 5 - abs(int(max.second) % 5) - y_min + 1;
        
        this->points = values;
        this->result = result;
        
        glOrtho(0.0, x_coord + 1, 0.0, y_coord + 1, -1.0, 1.0);
        glutMainLoop();
    }
};
