#include "graphic.h"
#include "mapgenerator.h"

using namespace std;

const Color Graphic::backgroundColor = Color(0.0, 0.0, 0.0);
const Color Graphic::wallColor = Color(0.0, 0.2, 1.0);

const char* const Graphic::gameTitle = "Food Collection Game - Tuita Team";

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

void Graphic::setGlutDimensions(int width, int height) {
    this->width = width;
    this->height = height;
}

void Graphic::glutInitialize(int *argcp, char **argv) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);
    glutCreateWindow(Graphic::gameTitle);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width-1, 0, height-1);
}

void Graphic::glutRun() {
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}

void Graphic::glutDisplay() {
    int cellWidth = width/map->getNumberOfCols();
    int cellHeight = height/map->getNumberOfRows();
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
                    glVertex2i(col*cellWidth, row*cellHeight);
                    glVertex2i(col*cellWidth, (row+1)*cellHeight);
                    glVertex2i((col+1)*cellWidth, (row+1)*cellHeight);
                    glVertex2i((col+1)*cellWidth, row*cellHeight);
                    glEnd();
                    break;
                case Corridor:
                    break;
            }
        }
    }
    glutSwapBuffers();
}

void Graphic::glutKeyboard() {
    Map m = MapGenerator(map->getNumberOfRows(), map->getNumberOfCols()).getMap();
    setMap(m);
    glutPostRedisplay();
}

void display() {
    Graphic::getInstance().glutDisplay();
}

void keyboard(unsigned char c, int x, int y) {
    Graphic::getInstance().glutKeyboard();
}