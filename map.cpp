#include "map.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Point::Point(const int row, const int col) : row(row), col(col){}

bool Point::operator<(const Point &p) const {
    return (row == p.row) ? col < p.col : row < p.row;
}

Map::Map(const int nRows, const int nCols) : nRows(nRows), nCols(nCols) {
    cells.assign(nRows, vector<CellType>(nCols, Wall));
}

bool Map::allCellsVisited(set<Point> &visited) {
    static const unsigned int visitable_cells =
            (nRows-1) / 2 * (nCols-1) / 2;
    return visited.size() == visitable_cells;
}

void Map::copySubMap(Map &m, Point startingPoint) {
    for (unsigned int i = 0; i < m.cells.size(); i += 1) {
        for (unsigned int j = 0; j < m.cells[i].size(); j += 1) {
            cells[startingPoint.row+i][startingPoint.col+j] = m.cells[i][j];
        }
    }
}

list<Point> Map::getNeighbors(Point &p) {
    list<Point> neighbors;
    if (p.col-2 >= 0) neighbors.push_back(Point(p.row, p.col-2));
    if (p.col+2 < nCols) neighbors.push_back(Point(p.row, p.col+2));
    if (p.row-2 >= 0) neighbors.push_back(Point(p.row-2, p.col));
    if (p.row+2 < nRows) neighbors.push_back(Point(p.row+2, p.col));
    return neighbors;
}

void Map::removeWall(Point &p, Point &neighbor) {
    cells[p.row][p.col] = Corridor;
    if (neighbor.row > p.row) {
        cells[p.row+1][p.col] = Corridor;
    } else if (neighbor.row < p.row) {
        cells[p.row-1][p.col] = Corridor;
    } else if (neighbor.col > p.col) {
        cells[p.row][p.col+1] = Corridor;
    } else if (neighbor.col < p.col) {
        cells[p.row][p.col-1] = Corridor;
    }
}

list<Point> Map::getUnvisitedNeighbors(Point &p, set<Point> &visitedCells) {
    list<Point> unvisitedNeighbors;
    list<Point> neighbors = getNeighbors(p);
    for (list<Point>::iterator n = neighbors.begin(); n != neighbors.end(); ++n) {
        if (visitedCells.count(*n) == 0) unvisitedNeighbors.push_back(*n);
    }
    return unvisitedNeighbors;
}

Point Map::getRandomPointOfList(list<Point> &points) {
    int randomElement = rand() % points.size();
    list<Point>::iterator p = points.begin();
    for (int i = 0; i < randomElement; ++p, i += 1) {}
    return *p;
}

void Map::copySymmetricLeftToRight() {
    for (int row = 1; row < nRows-1; row += 1) {
        for (int col = 1; col < nCols/2; col += 1) {
            cells[row][nCols-1 - col] = cells[row][col];
        }
    }
}

int Map::getNumberOfRows() {
    return nRows;
}

int Map::getNumberOfCols() {
    return nCols;
}

void Map::setSubColCellType(int col, int rowBegin, int rowEnd, CellType ct) {
    for (int i = rowBegin; i < rowEnd; i += 1) {
        cells[i][col] = ct;
    }
}

void Map::setAreaCellsType(Point &initialPoint, Point &finalPoint, CellType ct) {
    for (int row = initialPoint.row; row < finalPoint.row; row += 1) {
        for (int col = initialPoint.col; col < finalPoint.col; col += 1) {
            cells[row][col] = ct;
        }
    }
}

void Map::print() {
    for (int row = 0; row < nRows; row += 1) {
        for (int col = 0; col < nCols; col += 1) {
            switch (cells[row][col]) {
                case Wall:
                    cout << "0 ";
                    break;
                case Corridor:
                    cout << "· ";
                    break;
            }
        }
        cout << endl;
    }
}
