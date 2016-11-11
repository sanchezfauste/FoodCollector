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
    set<Point> foodCells;

    void initializeAvailableFood();
    void eatFood(Point p, CellType player);
    list<Direction> getEnemyLegalMoves();
    list<Direction> getPlayerLegalMoves();

  public:
    static const int minRows;
    static const int minCols;

    Map(const int nRows, const int nCols);
    Map(const int nRows, const int nCols, const Point playerInitialPosition,
            const Point enemyInitialPosition);
    Map(const Map &m);
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
    Point getPlayerPosition() const;
    Point getEnemyPosition() const;
    void playerMove(Direction d);
    void enemyMove(Direction d);
    bool playerCanMoveTo(Direction d);
    bool enemyCanMoveTo(Direction d);
    CellType getPointCellType(Point p);
    Point getNeighborPoint(Point p, Direction d);
    int getEatedFoodByPlayer();
    int getEatedFoodByEnemy();
    void initGame();
    Direction getCurrentPlayerDirection();
    Direction getNextPlayerDirection();
    void setCurrentPlayerDirection(Direction d);
    void setNextPlayerDirection(Direction d);
    list<Direction> getLegalMoves(CellType agent);
    bool isFoodAvailable();
    Map generateSuccessor(CellType agent, Direction action);
    set<Point> getFoodCells();

};

#endif
