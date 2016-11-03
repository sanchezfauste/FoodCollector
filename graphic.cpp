/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "graphic.h"
#include "mapgenerator.h"

using namespace std;

const Color Graphic::backgroundColor = Color(0.0, 0.0, 0.0);
const Color Graphic::wallColor = Color(0.0, 0.2, 1.0);
const Color Graphic::foodColor = Color(1.0, 0.64, 0);

const char* const Graphic::gameTitle = "Food Collection Game - Tuita Team";

const int Graphic::cellWidth = 30;
const int Graphic::cellHeight = 30;
const int Graphic::foodWidth = 6;
const int Graphic::foodHeight = 6;
const int Graphic::foodWidthPadding = (Graphic::cellWidth - Graphic::foodWidth) / 2;
const int Graphic::foodHeightPadding = (Graphic::cellHeight - Graphic::foodHeight) / 2;

Color::Color(const GLfloat red, const GLfloat green, const GLfloat blue) :
        red(red), green(green), blue(blue) {}

Graphic::Graphic() {}

Graphic& Graphic::getInstance() {
    static Graphic instance;
    return instance;
}

void Graphic::setMap(Map& map) {
    if (this->map) {
        delete(this->map);
    }
    this->map = new Map(map);
}

int Graphic::getScreenWidth() {
    return glutGet(GLUT_SCREEN_WIDTH);
}

int Graphic::getScreenHeight() {
    return glutGet(GLUT_SCREEN_HEIGHT);
}

void Graphic::setGlutDimensions(int width, int height) {
    this->width = width;
    this->height = height;
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);
    glutCreateWindow(Graphic::gameTitle);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width-1, 0, height-1);
}

void Graphic::glutInitialize(int *argcp, char **argv) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
}

void Graphic::glutRun() {
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}

void Graphic::glutDisplay() {
    glClearColor(Graphic::backgroundColor.red, Graphic::backgroundColor.green,
            Graphic::backgroundColor.blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int row = 0; row < map->getNumberOfRows(); row += 1) {
        vector<CellType> rowCells = map->getRow(map->getNumberOfRows()-1-row);
        for (int col = 0; col < map->getNumberOfCols(); col += 1) {
            switch (rowCells[col]) {
                case Wall:
                    glColor3f(Graphic::wallColor.red, Graphic::wallColor.green,
                            Graphic::wallColor.blue);
                    glBegin(GL_QUADS);
                    glVertex2i(col*Graphic::cellWidth, row*Graphic::cellHeight);
                    glVertex2i(col*Graphic::cellWidth, (row+1)*Graphic::cellHeight);
                    glVertex2i((col+1)*Graphic::cellWidth, (row+1)*Graphic::cellHeight);
                    glVertex2i((col+1)*Graphic::cellWidth, row*Graphic::cellHeight);
                    glEnd();
                    break;
                case Corridor:
                    break;
                case Food:
                    glColor3f(Graphic::foodColor.red, Graphic::foodColor.green,
                            Graphic::foodColor.blue);
                    glBegin(GL_QUADS);
                    glVertex2i(col*Graphic::cellWidth + Graphic::foodWidthPadding,
                            row*Graphic::cellHeight + Graphic::foodHeightPadding);
                    glVertex2i(col*Graphic::cellWidth + Graphic::foodWidthPadding,
                            (row+1)*Graphic::cellHeight - Graphic::foodHeightPadding);
                    glVertex2i((col+1)*Graphic::cellWidth - Graphic::foodWidthPadding,
                            (row+1)*Graphic::cellHeight - Graphic::foodHeightPadding);
                    glVertex2i((col+1)*Graphic::cellWidth - Graphic::foodWidthPadding,
                            row*Graphic::cellHeight + Graphic::foodHeightPadding);
                    glEnd();
                    break;
            }
        }
    }
    glutSwapBuffers();
}

void Graphic::glutKeyboard(unsigned char key, int x, int y) {
    Map m = MapGenerator(map->getNumberOfRows(), map->getNumberOfCols()).getMap();
    setMap(m);
    glutPostRedisplay();
}

void display() {
    Graphic::getInstance().glutDisplay();
}

void keyboard(unsigned char c, int x, int y) {
    Graphic::getInstance().glutKeyboard(c, x, y);
}
