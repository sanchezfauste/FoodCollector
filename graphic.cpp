/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "graphic.h"
#include "mapgenerator.h"
#include <sstream>

using namespace std;

const Color Graphic::backgroundColor = Color(0.0, 0.0, 0.0);
const Color Graphic::wallColor = Color(0.0, 0.2, 1.0);
const Color Graphic::foodColor = Color(1.0, 0.64, 0);
const Color Graphic::playerColor = Color(0.13, 0.54, 0.13);
const Color Graphic::enemyColor = Color(0.7, 0.13, 0.13);
const Color Graphic::textColor = Color(1.0, 1.0, 1.0);
const Size Graphic::scoreInfoPosition = Size(7.5, 7.5);

const char* const Graphic::gameTitle = "Food Collection Game - Tuita Team";

const int Graphic::cellWidth = 30;
const int Graphic::cellHeight = 30;
const int Graphic::foodWidth = 6;
const int Graphic::foodHeight = 6;
const int Graphic::foodWidthPadding = (Graphic::cellWidth - Graphic::foodWidth) / 2;
const int Graphic::foodHeightPadding = (Graphic::cellHeight - Graphic::foodHeight) / 2;
const int Graphic::playerWidth = 10;
const int Graphic::playerHeight = 10;
const int Graphic::playerWidthPadding =
        (Graphic::cellWidth - Graphic::playerWidth) / 2;
const int Graphic::playerHeightPadding =
        (Graphic::cellHeight - Graphic::playerHeight) / 2;
const long Graphic::playerMovementTime = 200;

Color::Color(const GLfloat red, const GLfloat green, const GLfloat blue) :
        red(red), green(green), blue(blue) {}

Size::Size(const float width, const float height) : width(width), height(height){}

Graphic::Graphic() : playerParticle(Particle()), enemyParticle(Particle()),
        enemyStrategy(NULL){}

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
    glutSpecialFunc(keyboardSpecial);
    glutIdleFunc(idle);
    lastTime = 0;
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
                    Graphic::drawSquareWithPadding(row, col, Graphic::cellWidth,
                            Graphic::cellHeight, 0, 0, Graphic::wallColor);
                    break;
                case Corridor:
                    break;
                case Food:
                    Graphic::drawSquareWithPadding(row, col, Graphic::cellWidth,
                            Graphic::cellHeight, Graphic::foodWidthPadding,
                            Graphic::foodHeightPadding, Graphic::foodColor);
                    break;
                default:
                    break;
            }
        }
    }
    Point playerPos = map->getPlayerPosition();
    Point enemyPos = map->getEnemyPosition();
    printPlayer(map->getNumberOfRows()-1-playerPos.row, playerPos.col,
            playerParticle, Graphic::playerColor);
    printPlayer(map->getNumberOfRows()-1-enemyPos.row, enemyPos.col,
            enemyParticle, Graphic::enemyColor);
    printScore(Graphic::scoreInfoPosition.width, Graphic::scoreInfoPosition.height);
    glutSwapBuffers();
}

void Graphic::printText(float width, float height, string str) {
    glColor3f(Graphic::textColor.red, Graphic::textColor.green,
            Graphic::textColor.blue);
    glRasterPos2f(width, height);
    for (int i = 0; i < (int) str.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
    }
}

void Graphic::printScore(float width, float height) {
    ostringstream convert;
    convert << "Player: " << map->getEatedFoodByPlayer() << " | Enemy: "
            << map->getEatedFoodByEnemy();
    printText(Graphic::scoreInfoPosition.width,
            Graphic::scoreInfoPosition.height, convert.str());
}

void Graphic::printPlayer(int row, int col, Particle &particle, Color color) {
    switch (particle.getState()) {
        case Quiet:
            Graphic::drawSquareWithPadding(row, col, Graphic::cellWidth,
                    Graphic::cellHeight, Graphic::playerWidthPadding,
                    Graphic::playerHeightPadding, color);
            break;
        case Moving:
            Graphic::drawSquareWithPadding(row, col, Graphic::cellWidth,
                    Graphic::cellHeight, Graphic::playerWidthPadding,
                    Graphic::playerHeightPadding, color, particle);
            break;
    }
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
        if (map->playerCanMoveTo(d)) {
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
        if (map->enemyCanMoveTo(d)) {
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

void Graphic::drawSquareWithPadding(int row, int col, int width, int height,
            int widthPadding, int heightPadding, Color color) {
    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex2i(col*width + widthPadding, row*height + heightPadding);
    glVertex2i(col*width + widthPadding, (row+1)*height - heightPadding);
    glVertex2i((col+1)*width - widthPadding, (row+1)*height - heightPadding);
    glVertex2i((col+1)*width - widthPadding, row*height + heightPadding);
    glEnd();
}

void Graphic::drawSquareWithPadding(int row, int col, int width, int height,
        int widthPadding, int heightPadding, Color color, Particle &p) {
    long widthTranslation = p.getCurrentWidthTranslation();
    long heightTranslation = p.getCurrentHeightTranslation();
    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);
    glVertex2i(col*width + widthPadding + widthTranslation, row*height + heightPadding + heightTranslation);
    glVertex2i(col*width + widthPadding + widthTranslation, (row+1)*height - heightPadding + heightTranslation);
    glVertex2i((col+1)*width - widthPadding + widthTranslation, (row+1)*height - heightPadding + heightTranslation);
    glVertex2i((col+1)*width - widthPadding + widthTranslation, row*height + heightPadding + heightTranslation);
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
        } else {
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
