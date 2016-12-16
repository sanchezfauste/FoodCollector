/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include "map.h"
#include "particle.h"
#include "tank_particle.h"
#include "enemy_strategy.h"
#include <GL/glut.h>
#include <string>

using namespace std;

typedef struct Color {
    const GLfloat red;
    const GLfloat green;
    const GLfloat blue;
    Color(const GLfloat red, const GLfloat green, const GLfloat blue);
} Color;

typedef struct Size {
    const float width;
    const float height;
    Size(const float width, const float height);
} Size;

typedef struct Point {
    const GLfloat x;
    const GLfloat y;
    const GLfloat z;
    Point(const GLfloat x, const GLfloat y, const GLfloat z);
} Point;

void display();
void keyboard(unsigned char c, int x, int y);
void keyboardSpecial(int key, int x, int y);
void idle();

class Graphic {

    static const Color backgroundColor;
    static const Color foodColor;
    static const Color playerColor;
    static const Color enemyColor;
    static const Color tankColor;
    static const Color tankWeelsColor;
    static const Color tankCanonColor;
    static const Color textColor;
    static const char* const gameTitle;
    static const Size scoreInfoPosition;
    static const double glutRatio;
    static const Direction defaultPlayerTankDirection;
    static const Direction defaultEnemyTankDirection;

    Map* map;
    int width;
    int height;
    int glutWidth;
    int glutHeight;
    TankParticle playerParticle;
    TankParticle enemyParticle;
    long lastTime;
    EnemyStrategy *enemyStrategy;
    int angleAlpha;
    int angleBeta;

    Graphic();
    Graphic(Graphic const&);
    void operator=(Graphic const&);
    void playerMove(Direction d);
    void printPlayer(int row, int col, TankParticle &particle, Color color);
    void enemyMove(Direction d);

  public:
    static const int cellWidth;
    static const int cellHeight;
    static const int cellDepth;
    static const GLfloat tankWidth;
    static const GLfloat tankHeight;
    static const GLfloat tankDepth;
    static const int foodRadius;
    static const GLint sphereSlices;
    static const GLint sphereStacks;
    static const GLint cylinderSlices;
    static const GLint cylinderStacks;
    static const long playerMovementTime;

    static Graphic& getInstance();
    static Size getTranslation(Direction d);
    void setMap(Map& map);
    void setGlutDimensions(int width, int height);
    void glutInitialize(int *argcp, char **argv);
    void glutRun();
    void glutDisplay();
    void glutKeyboard(unsigned char key, int x, int y);
    void glutKeyboardSpecial(int key, int x, int y);
    void glutIdle();
    int getScreenWidth();
    int getScreenHeight();
    void drawWall(int row, int col);
    void drawFood(int row, int col);
    void drawTank(int row, int col, TankParticle &p, Color color);
    void drawCylinder(Color color, Point p, GLdouble radius, GLdouble height);
    void drawSphere(Color color, Point p, GLdouble radius);
    void drawCube(Color color, Point p, GLfloat width, GLfloat height, GLfloat depth);
    void printText(float width, float height, string str);
    void printScore(float width, float height);
    void initDisplay();
    void positionObserver(float alpha, float beta, int radius);
    void drawFloor(int row, int col);

};

#endif
