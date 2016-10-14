#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include "map.h"
#include <GL/glut.h>

using namespace std;

typedef struct Color {
    const GLfloat red;
    const GLfloat green;
    const GLfloat blue;
    Color(const GLfloat red, const GLfloat green, const GLfloat blue);
} Color;

void display();
void keyboard(unsigned char c, int x, int y);

class Graphic {

    static const Color backgroundColor;
    static const Color wallColor;
    static const char* const gameTitle;

    Map* map;
    int width;
    int height;

    Graphic();
    Graphic(Graphic const&);
    void operator=(Graphic const&);

  public:
    static Graphic& getInstance();
    void setMap(Map& map);
    void setGlutDimensions(int width, int height);
    void glutInitialize(int *argcp, char **argv);
    void glutRun();
    void glutDisplay();
    void glutKeyboard(unsigned char key, int x, int y);

};

#endif
