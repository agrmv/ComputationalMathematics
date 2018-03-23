//g++  -std=c++0x main.cpp -lglut -lGL -lGLU -lGLEW
#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>

using vector_t = std::vector<double>;
extern const int16_t xMin = -2;
extern const int16_t xMax =  2;
extern const int16_t yMin = -2;
extern const int16_t yMax =  2;
extern const int16_t leftmost  = -1;
extern const int16_t rightmost =  1;
extern const int16_t uppermost =  1;
extern const int16_t lowermost = -1;
extern const double step 	   = 0.5;


void draw_string_bitmap(void *font, const char* string) 
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}

double q(const vector_t &vec, double x, size_t n) {
    if (n >= vec.size())
        return 1;
    double result = 1;
    for (size_t i = 0; i < vec.size(); ++i)
        if (i != n)
            result *= (x - vec[i]) / (vec[n] - vec[i]);
    return result;
}

double p(const vector_t &_x, const vector_t &y, double x, size_t n) {
    double res = 0;
    for (size_t i = 0; i <= n; ++i)
        res += y[i] * q(_x, x, i);
    return res;
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
    size_t n = 4;
    vector_t _x{-1.0, -0.6, -0.3, 0.3, 1.0};
    vector_t  y{ 0.5,  1.0,  0.4, 0.1, 0.5};
    
    glClear(GL_COLOR_BUFFER_BIT);   
    glColor3f(0.0, 0.0, 0.0);
    //OX
    glBegin(GL_LINES);
		glVertex2f(xMin, 0);   
		glVertex2f(xMax, 0); 
		for (double i = leftmost; i <= rightmost; i += step) {
			glVertex2f(-0.05, i);
			glVertex2f( 0.05, i);
		} 
    glEnd();
    
    for (double i = leftmost; i <= rightmost; i += step) {
		glRasterPos2f(i, -0.2);
		std::ostringstream ch;
		ch << i;
		draw_string_bitmap(GLUT_BITMAP_HELVETICA_18, ch.str().c_str());
	}
    //OY
    glBegin(GL_LINES);
		glVertex2f(0, yMin);
		glVertex2f(0, yMax);
		for (double i = leftmost; i <= rightmost; i += step) {
			glVertex2f(i, -0.05);
			glVertex2f(i,  0.05);
		} 
    glEnd();
	
	for (double i = leftmost; i <= rightmost; i += step) {
		if (i == 0) continue;
		glRasterPos2f(0.1, i);
		std::ostringstream ch;
		ch << i;
		draw_string_bitmap(GLUT_BITMAP_HELVETICA_18, ch.str().c_str());
	}
    
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
    gluOrtho2D(xMin, xMax, yMin, yMax); 
    glutDisplayFunc(display); 
    glutMainLoop(); 
    return 0;
}
