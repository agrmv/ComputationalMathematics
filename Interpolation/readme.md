# Набираем в терминале:
```
$ sudo apt-get install freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-   mesa-dev mesa-common-dev
```
# Подключение заголовочного файла:
#include <GL/freeglut.h>

# При компиляции использовать команду:
g++ -std=c++0x main.cpp -lglut -lGL -lGLU -lGLEW
