/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "mapgenerator.h"
#include <cstdlib>
#include <ctime>
#include <stack>

using namespace std;

MapGenerator::MapGenerator(const int nRows, const int nCols) :
        map(Map(nRows - ((nRows % 2 == 0) ? 1 : 0),
        nCols / 2 + nCols % 2)), nRows(nRows), nCols(nCols) {
    generate();
}

Map MapGenerator::getMap() {
    Map m = Map(nRows, nCols, Point(1, 1), Point(1, nCols - 2));
    m.copySubMap(map, Point(0, 0));
    m.setSubColCellType(map.getNumberOfCols() - 1,
            1, map.getNumberOfRows() - 1, Food);
    m.copySymmetricLeftToRight();
    m.initGame();
    return m;
}

void MapGenerator::generate() {
    srand(time(NULL));
    set<Point> visited;
    stack<Point> stk;
    Point *current = new Point(1, 1);
    stk.push(*current);
    visited.insert(*current);

    while(!stk.empty()) {
        list<Point> unvisitedNeighbors = map.getUnvisitedNeighbors(*current, visited);
        if (unvisitedNeighbors.empty()) {
            current = new Point(stk.top());
            list<Point> neighbors = map.getNeighbors(*current);
            Point p = map.getRandomPointOfList(neighbors);
            map.removeWall(*current, p);
            stk.pop();
        } else {
            Point *neighbor = new Point(map.getRandomPointOfList(unvisitedNeighbors));
            map.removeWall(*current, *neighbor);
            delete(current);
            current = neighbor;
            visited.insert(*current);
            stk.push(*current);
        }
    }
}
