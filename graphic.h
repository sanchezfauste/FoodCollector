/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include "map.h"
#include "particle.h"
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
void idle();

class Graphic {

    static const Color backgroundColor;
    static const Color wallColor;
    static const Color foodColor;
    static const Color playerColor;
    static const Color enemyColor;
    static const char* const gameTitle;

    Map* map;
    int width;
    int height;
    Particle playerParticle;
    Particle enemyParticle;
    long lastTime;

    Graphic();
    Graphic(Graphic const&);
    void operator=(Graphic const&);
    void playerMove(Direction d);
    void printPlayer(int row, int col);

  public:
    static const int cellWidth;
    static const int cellHeight;
    static const int foodWidth;
    static const int foodHeight;
    static const int foodWidthPadding;
    static const int foodHeightPadding;
    static const int playerWidth;
    static const int playerHeight;
    static const int playerWidthPadding;
    static const int playerHeightPadding;
    static const long playerMovementTime;

    static Graphic& getInstance();
    void setMap(Map& map);
    void setGlutDimensions(int width, int height);
    void glutInitialize(int *argcp, char **argv);
    void glutRun();
    void glutDisplay();
    void glutKeyboard(unsigned char key, int x, int y);
    void glutIdle();
    int getScreenWidth();
    int getScreenHeight();
    static void drawSquareWithPadding(int row, int col, int width, int height,
            int widthPadding, int heightPadding, Color color);
    static void drawSquareWithPadding(int row, int col, int width, int height,
            int widthPadding, int heightPadding, Color color, Particle &p);

};

#endif
