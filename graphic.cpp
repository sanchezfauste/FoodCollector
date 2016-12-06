/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "graphic.h"
#include "mapgenerator.h"
#include <sstream>
#include <math.h>

using namespace std;

const Color Graphic::backgroundColor = Color(0.15, 0.15, 0.15);
const Color Graphic::wallColor = Color(0.0, 0.70, 0.3);
const Color Graphic::wallSideColor = Color(0.0, 0.35, 0.15);
const Color Graphic::foodColor = Color(1.0, 0.64, 0);
const Color Graphic::playerColor = Color(0.13, 0.54, 0.13);
const Color Graphic::enemyColor = Color(0.7, 0.13, 0.13);
const Color Graphic::textColor = Color(1.0, 1.0, 1.0);
const Color Graphic::tankColor = Color(0, 0, 0);
const Color Graphic::tankWeelsColor = Color(0.38, 0.38, 0.38);
const Color Graphic::tankCanonColor = Color(0, 0, 0);
const Size Graphic::scoreInfoPosition = Size(7.5, 7.5);
const double Graphic::glutRatio = 1.1;

const char* const Graphic::gameTitle = "Food Collection Game - Tuita Team";

const int Graphic::cellWidth = 30;
const int Graphic::cellHeight = 30;
const int Graphic::cellDepth = 8;
const int Graphic::foodRadius = 5;
const GLint Graphic::sphereSlices = 10;
const GLint Graphic::sphereStacks = 10;
const GLint Graphic::cylinderSlices = 10;
const GLint Graphic::cylinderStacks = 10;
const long Graphic::playerMovementTime = 150;

Color::Color(const GLfloat red, const GLfloat green, const GLfloat blue) :
        red(red), green(green), blue(blue) {}

Size::Size(const float width, const float height) : width(width), height(height){}

Point::Point(const GLfloat x, const GLfloat y, const GLfloat z) : x(x), y(y), z(z) {}

Graphic::Graphic() : playerParticle(Particle()), enemyParticle(Particle()),
        enemyStrategy(NULL), angleAlpha(270), angleBeta(60){}

Graphic& Graphic::getInstance() {
    static Graphic instance;
    return instance;
}

void Graphic::setMap(Map& map) {
    if (this->map) {
        delete(this->map);
    }
    if (enemyStrategy == NULL) delete(enemyStrategy);
    this->map = new Map(map);
    enemyStrategy = new EnemyStrategy(this->map);
    playerParticle.setState(Quiet);
    enemyParticle.setState(Quiet);
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
    glutWidth = (width-1)*Graphic::glutRatio;
    glutHeight = (height-1)*Graphic::glutRatio;
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);
    glutCreateWindow(Graphic::gameTitle);
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);
    glOrtho(-glutWidth/2, glutWidth/2, -glutHeight/2, glutHeight/2, 10, 2000);
}

void Graphic::glutInitialize(int *argcp, char **argv) {
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
}

void Graphic::glutRun() {
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutIdleFunc(idle);
    lastTime = 0;
    glutMainLoop();
}

void Graphic::glutDisplay() {
    initDisplay();
    for (int row = 0; row < map->getNumberOfRows(); row += 1) {
        vector<CellType> rowCells = map->getRow(map->getNumberOfRows()-1-row);
        for (int col = 0; col < map->getNumberOfCols(); col += 1) {
            switch (rowCells[col]) {
                case Wall:
                    drawWall(row, col);
                    break;
                case Corridor:
                    break;
                case Food:
                    Graphic::drawFood(row, col);
                    break;
                default:
                    break;
            }
        }
    }
    Position playerPos = map->getPlayerPosition();
    Position enemyPos = map->getEnemyPosition();
    printPlayer(map->getNumberOfRows()-1-playerPos.row, playerPos.col,
            playerParticle, Graphic::playerColor);
    printPlayer(map->getNumberOfRows()-1-enemyPos.row, enemyPos.col,
            enemyParticle, Graphic::enemyColor);
    printScore(Graphic::scoreInfoPosition.width, Graphic::scoreInfoPosition.height);
    glutSwapBuffers();
}

void Graphic::printText(float width, float height, string str) {
    glPushMatrix();
    glTranslatef(width, height, Graphic::cellDepth/2 + 1);
    glColor3f(Graphic::textColor.red, Graphic::textColor.green,
            Graphic::textColor.blue);
    double scale = (double) Graphic::cellWidth/3 / glutStrokeWidth(GLUT_STROKE_ROMAN, 'A');
    glScalef(scale, scale, 0);
    for (int i = 0; i < (int) str.size(); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void Graphic::printScore(float width, float height) {
    ostringstream convert;
    convert << "Player: " << map->getEatedFoodByPlayer() << " | Enemy: "
            << map->getEatedFoodByEnemy();
    printText(-this->width/2 + width, -this->height/2 + height, convert.str());
}

void Graphic::printPlayer(int row, int col, Particle &particle, Color color) {
    drawTank(row, col, particle, color);
}

void Graphic::glutKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            playerMove(Up);
            break;
        case 's':
            playerMove(Down);
            break;
        case 'a':
            playerMove(Left);
            break;
        case 'd':
            playerMove(Right);
            break;
        case 'i':
            if (angleBeta <= (90 - 4)) angleBeta += 3;
            break;
        case 'k':
            if (angleBeta >= (-90 + 4)) angleBeta -= 3;
            break;
        case 'j':
            angleAlpha = (angleAlpha + 3) % 360;
            break;
        case 'l':
            angleAlpha = (angleAlpha - 3 + 360) % 360;
            break;
        default:
            Map m = MapGenerator(map->getNumberOfRows(), map->getNumberOfCols()).getMap();
            setMap(m);
            break;
    }
    glutPostRedisplay();
}

void Graphic::glutKeyboardSpecial(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            playerMove(Up);
            break;
        case GLUT_KEY_DOWN:
            playerMove(Down);
            break;
        case GLUT_KEY_LEFT:
            playerMove(Left);
            break;
        case GLUT_KEY_RIGHT:
            playerMove(Right);
            break;
        default:
            break;
    }
}

void Graphic::playerMove(Direction d) {
    if (playerParticle.getState() != Moving) {
        if (map->playerCanMoveTo(d) && d != None) {
            map->setCurrentPlayerDirection(d);
            Size translation = Graphic::getTranslation(d);
            playerParticle.initMovement(translation.width, translation.height,
                    Graphic::playerMovementTime, d);
        }
    } else {
        map->setNextPlayerDirection(d);
    }
}

void Graphic::enemyMove(Direction d) {
    if (enemyParticle.getState() != Moving) {
        if (map->enemyCanMoveTo(d) && d != None) {
            Size translation = Graphic::getTranslation(d);
            enemyParticle.initMovement(translation.width, translation.height,
                    Graphic::playerMovementTime, d);
        }
    }
}

Size Graphic::getTranslation(Direction d) {
    float widthTranslation = 0;
    float heightTranslation = 0;
    switch (d) {
        case Up:
            heightTranslation = Graphic::cellHeight;
            break;
        case Down:
            heightTranslation = - Graphic::cellHeight;
            break;
        case Left:
            widthTranslation = - Graphic::cellWidth;
            break;
        case Right:
            widthTranslation = Graphic::cellWidth;
            break;
        default:
            break;
    }
    return Size(widthTranslation, heightTranslation);
}

void display() {
    Graphic::getInstance().glutDisplay();
}

void keyboard(unsigned char c, int x, int y) {
    Graphic::getInstance().glutKeyboard(c, x, y);
}

void keyboardSpecial(int key, int x, int y) {
    Graphic::getInstance().glutKeyboardSpecial(key, x, y);
}

void idle() {
    Graphic::getInstance().glutIdle();
}

void Graphic::drawWall(int row, int col) {
    int x = (col + 0.5) * Graphic::cellWidth - this->width/2;
    int y = (row + 0.5) * Graphic::cellHeight - this->height/2;
    int z = Graphic::cellDepth;
    int x1 = Graphic::cellWidth/2;
    int y1 = Graphic::cellHeight/2;
    int z1 = z/2;

    glColor3f(Graphic::wallColor.red, Graphic::wallColor.green,
        Graphic::wallColor.blue);
    glBegin(GL_QUADS);
    glVertex3i(x + x1, y + y1, z1);
    glVertex3i(x - x1, y + y1, z1);
    glVertex3i(x - x1, y - y1, z1);
    glVertex3i(x + x1, y - y1, z1);
    glEnd();

    glColor3f(Graphic::wallSideColor.red, Graphic::wallSideColor.green,
        Graphic::wallSideColor.blue);
    glBegin(GL_QUADS);
    glVertex3i(x + x1, y + y1, z1 - 1);
    glVertex3i(x - x1, y + y1, z1 - 1);
    glVertex3i(x - x1, y - y1, z1 - 1);
    glVertex3i(x + x1, y - y1, z1 - 1);
    glEnd();

    glColor3f(Graphic::wallSideColor.red, Graphic::wallSideColor.green,
        Graphic::wallSideColor.blue);
    glBegin(GL_QUADS);
    glVertex3i(x + x1, y - y1, -z1);
    glVertex3i(x - x1, y - y1, -z1);
    glVertex3i(x - x1, y + y1, -z1);
    glVertex3i(x + x1, y + y1, -z1);
    glEnd();

    glColor3f(Graphic::wallSideColor.red, Graphic::wallSideColor.green,
        Graphic::wallSideColor.blue);
    glBegin(GL_QUADS);
    glVertex3i(x + x1, y - y1, z1 - 1);
    glVertex3i(x + x1, y - y1, -z1);
    glVertex3i(x + x1, y + y1, -z1);
    glVertex3i(x + x1, y + y1, z1 - 1);
    glEnd();

    glColor3f(Graphic::wallSideColor.red, Graphic::wallSideColor.green,
        Graphic::wallSideColor.blue);
    glBegin(GL_QUADS);
    glVertex3i(x - x1, y + y1, z1 - 1);
    glVertex3i(x - x1, y + y1, -z1);
    glVertex3i(x - x1, y - y1, -z1);
    glVertex3i(x - x1, y - y1, z1 - 1);
    glEnd();

    glColor3f(Graphic::wallSideColor.red, Graphic::wallSideColor.green,
        Graphic::wallSideColor.blue);
    glBegin(GL_QUADS);
    glVertex3i(x - x1, y + y1, -z1);
    glVertex3i(x - x1, y + y1, z1 - 1);
    glVertex3i(x + x1, y + y1, z1 - 1);
    glVertex3i(x + x1, y + y1, -z1);
    glEnd();

    glColor3f(Graphic::wallSideColor.red, Graphic::wallSideColor.green,
        Graphic::wallSideColor.blue);
    glBegin(GL_QUADS);
    glVertex3i(x + x1, y - y1, -z1);
    glVertex3i(x + x1, y - y1, z1 - 1);
    glVertex3i(x - x1, y - y1, z1 - 1);
    glVertex3i(x - x1, y - y1, -z1);
    glEnd();
}

void Graphic::drawFood(int row, int col) {
    int x = (col + 0.5) * Graphic::cellWidth - this->width/2;
    int y = (row + 0.5) * Graphic::cellHeight - this->height/2;
    glPushMatrix();
        glTranslatef(x, y, 0);
        glColor3f(Graphic::foodColor.red, Graphic::foodColor.green,
            Graphic::foodColor.blue);
        glutSolidSphere(Graphic::foodRadius, Graphic::sphereSlices,
            Graphic::sphereStacks);
    glPopMatrix ();
}

void Graphic::drawTank(int row, int col, Particle &p, Color color) {
    /*long widthTranslation = 0;
    long heightTranslation = 0;
    if (p.getState() != Quiet) {
        widthTranslation = p.getCurrentWidthTranslation();
        heightTranslation = p.getCurrentHeightTranslation();
    }
    int x = (col + 0.5) * Graphic::cellWidth - this->width/2 + widthTranslation;
    int y = (row + 0.5) * Graphic::cellHeight - this->height/2 + heightTranslation;
    glPushMatrix();
        glTranslatef(x, y, 0);
        glColor3f(color.red, color.green, color.blue);
        glutSolidSphere(Graphic::foodRadius, Graphic::sphereSlices,
            Graphic::sphereStacks);
    glPopMatrix ();*/

    GLfloat widthTranslation = 0;
    GLfloat heightTranslation = 0;
    if (p.getState() != Quiet) {
        widthTranslation = p.getCurrentWidthTranslation();
        heightTranslation = p.getCurrentHeightTranslation();
    }
    GLfloat x = (col + 0.5) * Graphic::cellWidth - this->width/2 + widthTranslation;
    GLfloat y = (row + 0.5) * Graphic::cellHeight - this->height/2 + heightTranslation;
    GLfloat z = 0;
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(30/300.0, 30/300.0, 30/300.0);
        glRotatef(-90, 0, 0, -1);
        //Draw the axis
        drawCylinder(Graphic::tankColor, Point(100, 0, -250/2.0), 25, 250);
        drawCylinder(Graphic::tankColor, Point(-100, 0, -250/2.0), 25, 250);
        //Axis hubcaps
        drawSphere(Graphic::tankColor, Point(-100, 250/2.0, 0), 25);
        drawSphere(Graphic::tankColor, Point(100, 250/2.0, 0), 25);
        drawSphere(Graphic::tankColor, Point(100, -250/2.0, 0), 25);
        drawSphere(Graphic::tankColor, Point(-100, -250/2.0, 0), 25);
        //Tank body
        drawCube(color, Point(0, 0, 25), 150, 150, 50);
        drawCube(Graphic::tankColor, Point(0, 0, 75), 75, 75, 60);
        //Canon
        drawCylinder(Graphic::tankCanonColor, Point(0, -60-25/2.0, 0),  25, 150);
        //Wheels
        drawSphere(Graphic::tankWeelsColor, Point(110, 40, 0), 20);
        drawSphere(Graphic::tankWeelsColor, Point(110, -40, 0), 20);
        drawSphere(Graphic::tankWeelsColor, Point(110, 90, 0), 20);
        drawSphere(Graphic::tankWeelsColor, Point(110, -90, 0), 20);
        drawSphere(Graphic::tankWeelsColor, Point(-110, 40, 0), 20);
        drawSphere(Graphic::tankWeelsColor, Point(-110, -40, 0), 20);
        drawSphere(Graphic::tankWeelsColor, Point(-110, 90, 0), 20);
        drawSphere(Graphic::tankWeelsColor, Point(-110, -90, 0), 20);
    glPopMatrix ();
}

void Graphic::drawCylinder(Color color, Point p, GLdouble radius, GLdouble height) {
    glPushMatrix();
        glColor3f(color.red, color.green, color.blue);
        glRotatef(-90, 1, 0, 0);
        glTranslatef(p.x, p.y, p.z);
        gluCylinder(gluNewQuadric(), radius, radius, height,
                Graphic::cylinderSlices, Graphic::cylinderStacks);
    glPopMatrix ();
}

void Graphic::drawSphere(Color color, Point p, GLdouble radius) {
    glPushMatrix();
        glTranslatef(p.x, p.y, p.z);
        glColor3f(color.red, color.green, color.blue);
        glutSolidSphere(radius, Graphic::sphereSlices, Graphic::sphereStacks);
    glPopMatrix ();
}

void Graphic::drawCube(Color color, Point p, GLfloat width, GLfloat height, GLfloat depth) {
    GLfloat x1 = width/2;
    GLfloat y1 = height/2;
    GLfloat z1 = depth/2;

    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex3f(p.x + x1, p.y + y1, p.z + z1);
    glVertex3f(p.x - x1, p.y + y1, p.z + z1);
    glVertex3f(p.x - x1, p.y - y1, p.z + z1);
    glVertex3f(p.x + x1, p.y - y1, p.z + z1);
    glEnd();

    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex3f(p.x + x1, p.y + y1, p.z + z1 - 1);
    glVertex3f(p.x - x1, p.y + y1, p.z + z1 - 1);
    glVertex3f(p.x - x1, p.y - y1, p.z + z1 - 1);
    glVertex3f(p.x + x1, p.y - y1, p.z + z1 - 1);
    glEnd();

    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex3f(p.x + x1, p.y - y1, p.z + -z1);
    glVertex3f(p.x - x1, p.y - y1, p.z + -z1);
    glVertex3f(p.x - x1, p.y + y1, p.z + -z1);
    glVertex3f(p.x + x1, p.y + y1, p.z + -z1);
    glEnd();

    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex3f(p.x + x1, p.y - y1, p.z + z1 - 1);
    glVertex3f(p.x + x1, p.y - y1, p.z + -z1);
    glVertex3f(p.x + x1, p.y + y1, p.z + -z1);
    glVertex3f(p.x + x1, p.y + y1, p.z + z1 - 1);
    glEnd();

    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex3f(p.x - x1, p.y + y1, p.z + z1 - 1);
    glVertex3f(p.x - x1, p.y + y1, p.z + -z1);
    glVertex3f(p.x - x1, p.y - y1, p.z + -z1);
    glVertex3f(p.x - x1, p.y - y1, p.z + z1 - 1);
    glEnd();

    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex3f(p.x - x1, p.y + y1, p.z + -z1);
    glVertex3f(p.x - x1, p.y + y1, p.z + z1 - 1);
    glVertex3f(p.x + x1, p.y + y1, p.z + z1 - 1);
    glVertex3f(p.x + x1, p.y + y1, p.z + -z1);
    glEnd();

    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex3f(p.x + x1, p.y - y1, p.z + -z1);
    glVertex3f(p.x + x1, p.y - y1, p.z + z1 - 1);
    glVertex3f(p.x - x1, p.y - y1, p.z + z1 - 1);
    glVertex3f(p.x - x1, p.y - y1, p.z + -z1);
    glEnd();
}

void Graphic::glutIdle() {
    long t = glutGet(GLUT_ELAPSED_TIME);
    if (lastTime == 0) {
        lastTime = t;
    } else {
        long elapsedTime = t - lastTime;
        if (playerParticle.getState() == Moving) {
            if (playerParticle.integrate(elapsedTime)) {
                Direction nextPlayerDirection = map->getNextPlayerDirection();
                map->playerMove(playerParticle.getDirection());
                if (nextPlayerDirection != None
                        && map->playerCanMoveTo(nextPlayerDirection)) {
                    playerMove(nextPlayerDirection);
                    map->setNextPlayerDirection(None);
                } else {
                    playerMove(map->getCurrentPlayerDirection());
                }
            }
        }
        if (enemyParticle.getState() == Moving) {
            if (enemyParticle.integrate(elapsedTime)) {
                map->enemyMove(enemyParticle.getDirection());
            }
        } else if (map->isFoodAvailable()) {
            Direction d = enemyStrategy->getAction();
            if (map->enemyCanMoveTo(d)) {
                Size translation = Graphic::getTranslation(d);
                enemyParticle.initMovement(translation.width,
                        translation.height, Graphic::playerMovementTime, d);
            }
        }
        lastTime = t;
    }
    glutPostRedisplay();
}

void Graphic::initDisplay() {
    glClearColor(Graphic::backgroundColor.red, Graphic::backgroundColor.green,
            Graphic::backgroundColor.blue, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    positionObserver(angleAlpha, angleBeta, 450);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
}

void Graphic::positionObserver(float alpha, float beta, int radius) {
    float x, y, z;
    float upx, upy, upz;
    float modul;

    x = (float) radius * cos(alpha*2*M_PI/360.0) * cos(beta*2*M_PI/360.0);
    y = (float) radius * sin(alpha*2*M_PI/360.0) * cos(beta*2*M_PI/360.0);
    z = (float) radius * sin(beta*2*M_PI/360.0);

    if (beta > 0) {
        upx = -x;
        upy = -y;
        upz = (x*x + y*y) / z;
    } else if (beta == 0) {
        upx=0;
        upy=0;
        upz=1;
    } else {
        upx = x;
        upy = y;
        upz = -(x*x + y*y) / z;
    }

    modul = sqrt(upx*upx + upy*upy + upz*upz);

    upx = upx/modul;
    upy = upy/modul;
    upz = upz/modul;

    gluLookAt(x, y, z, 0.0, 0.0, 0.0, upx, upy, upz);
}
