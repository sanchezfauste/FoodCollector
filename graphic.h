/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include "map.h"
#include "particle.h"
#include "tank_particle.h"
#include "strategy.h"
#include <GL/glut.h>
#include <string>

#ifdef ARDUINO
    #include "arduino/arduino.h"
    #include "texture_loader.h"
#endif

using namespace std;

typedef struct Color {
    const GLfloat red;
    const GLfloat green;
    const GLfloat blue;
    const GLfloat alpha;
    Color(const GLfloat red, const GLfloat green, const GLfloat blue,
        const GLfloat alpha = 1.0);
    GLfloat * getArray();
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
    static const GLfloat* const foodColor;
    static const GLfloat* const playerColor;
    static const GLfloat* const enemyColor;
    static const GLfloat* const tankColor;
    static const GLfloat* const tankWeelsColor;
    static const GLfloat* const tankCanonColor;
    static const GLfloat* const textColor;
    static const GLfloat* const headlightColor; 
    static const GLfloat* const fullColor;
    static const GLfloat* const ambientColor;
    static const GLfloat* const diffuseColor;
    static const GLfloat* const specularColor;
    static const GLfloat* const bulletColor;
    static const GLfloat* const spotLightColor;
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
    Particle bulletParticle;
    Position *bulletPosition;
    long lastTime;
    Strategy *enemyStrategy;
    Strategy *playerStrategy;
    int angleAlpha;
    int angleBeta;
    bool gameRunning;
    bool training;
    double speedFactor;

    #ifdef ARDUINO
        static const Size arduinoInfoPosition;

        Arduino *arduino;
        Texture waterTexture;
        int lastTemperature;
        int lastHeartRate;

        void arduinoController();
        static Texture choiseTextureFromTemperature(int temperatureInCelcius);
        void setSpeedFactorFromHeartRate(int heartRate);
        void printArduinoInfo();
    #endif

    Graphic(bool training = false);
    Graphic(Graphic const&);
    void operator=(Graphic const&);
    void playerMove(Direction d);
    void printPlayer(int row, int col, TankParticle &particle,
            const GLfloat* color, CellType player);
    void printBullet(int row, int col, Particle &particle, const GLfloat* color);
    void tankShoot();
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
    static const long bulletMovementTime;

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
    void drawTank(int row, int col, TankParticle &p, const GLfloat* color,
            CellType player);
    void drawCylinder(const GLfloat* color, Point p, GLdouble radius,
            GLdouble height);
    void drawSphere(const GLfloat* color, Point p, GLdouble radius);
    void drawCube(const GLfloat* color, Point p, GLfloat width, GLfloat height,
            GLfloat depth);
    void printText(float width, float height, string str);
    void printScore(float width, float height);
    void initDisplay();
    void positionObserver(float alpha, float beta, int radius);
    void drawFloor(int row, int col);
    void setSpotLight(GLenum light, Point p, const GLfloat* color);
    void setTraining(bool training);

};

#endif
