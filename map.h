/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __MAP_H
#define __MAP_H

#include <vector>
#include <set>
#include <list>

using namespace std;

enum CellType {
    Wall,
    Corridor,
    Food,
    Player,
    Enemy
};

enum Direction {
    Up,
    Down,
    Left,
    Right,
    None
};

typedef struct Point {
    const int row;
    const int col;
    Point(const int row, const int col);
    Point();
    bool operator<(const Point &p) const;
} Point;

class Map {

    vector<vector<CellType> > cells;
    const int nRows, nCols;
    Point *playerPosition;
    Point *enemyPosition;
    int eatedFoodByPlayer;
    int eatedFoodByEnemy;
    const Point playerInitialPosition;
    const Point enemyInitialPosition;
    int numberOfAvailableFood;
    Direction currentPlayerDirection;
    Direction nextPlayerDirection;

    int getNumberOfAvailableFood();
    void eatFood(Point p, CellType player);

  public:
    static const int minRows;
    static const int minCols;

    Map(const int nRows, const int nCols);
    Map(const int nRows, const int nCols, const Point playerInitialPosition,
            const Point enemyInitialPosition);
    void print();
    void copySymmetricLeftToRight();
    int getNumberOfRows();
    int getNumberOfCols();
    void copySubMap(Map &m, Point startingPoint);
    void setAreaCellsType(Point &initialPoint, Point &finalPoint, CellType ct);
    void setSubColCellType(int col, int rowBegin, int rowEnd, CellType ct);
    bool allCellsVisited(set<Point> &visited);
    list<Point> getNeighbors(Point &p);
    list<Point> getUnvisitedNeighbors(Point &p, set<Point> &visitedCells);
    Point getRandomPointOfList(list<Point> &points);
    void removeWall(Point &p, Point &neighbor);
    vector<CellType> getRow(int row);
    void setPlayerPosition(Point p);
    void setEnemyPosition(Point p);
    Point getPlayerPosition();
    Point getEnemyPosition();
    void playerMove(Direction d);
    bool playerCanMoveTo(Direction d);
    CellType getPointCellType(Point p);
    Point getNeighborPoint(Point p, Direction d);
    int getEatedFoodByPlayer();
    int getEatedFoodByEnemy();
    void initGame();
    Direction getCurrentPlayerDirection();
    Direction getNextPlayerDirection();
    void setCurrentPlayerDirection(Direction d);
    void setNextPlayerDirection(Direction d);

};

#endif
