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
    Right
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

  public:
    static const int minRows;
    static const int minCols;

    Map(const int nRows, const int nCols);
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

};

#endif
