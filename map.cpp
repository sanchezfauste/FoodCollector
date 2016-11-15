/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "map.h"
#include <iostream>
#include <cstdlib>

using namespace std;

const int Map::minRows = 3;
const int Map::minCols = 3;

Point::Point(const int row, const int col) : row(row), col(col){}

Point::Point() : row(-1), col(-1){}

bool Point::operator<(const Point &p) const {
    return (row == p.row) ? col < p.col : row < p.row;
}

Map::Map(const int nRows, const int nCols) : nRows(nRows), nCols(nCols),
        eatedFoodByPlayer(0), eatedFoodByEnemy(0),
        playerInitialPosition(Point(1, 1)),
        enemyInitialPosition(Point(1, nCols - 2)) {
    cells.assign(nRows, vector<CellType>(nCols, Wall));
    playerPosition = new Point();
    enemyPosition = new Point();
}

Map::Map(const int nRows, const int nCols, const Point playerInitialPosition,
        const Point enemyInitialPosition) : nRows(nRows), nCols(nCols),
                eatedFoodByPlayer(0), eatedFoodByEnemy(0),
                playerInitialPosition(playerInitialPosition),
                enemyInitialPosition(enemyInitialPosition) {
    cells.assign(nRows, vector<CellType>(nCols, Wall));
    playerPosition = new Point();
    enemyPosition = new Point();
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
    cells[p.row][p.col] = Food;
    if (neighbor.row > p.row) {
        cells[p.row+1][p.col] = Food;
    } else if (neighbor.row < p.row) {
        cells[p.row-1][p.col] = Food;
    } else if (neighbor.col > p.col) {
        cells[p.row][p.col+1] = Food;
    } else if (neighbor.col < p.col) {
        cells[p.row][p.col-1] = Food;
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

vector<CellType> Map::getRow(int row) {
    return cells[row];
}

void Map::print() {
    for (int row = 0; row < nRows; row += 1) {
        for (int col = 0; col < nCols; col += 1) {
            switch (cells[row][col]) {
                case Wall:
                    cout << "0 ";
                    break;
                case Food:
                    cout << "· ";
                    break;
                case Corridor:
                    cout << "  ";
                    break;
                case Player:
                    cout << "P ";
                    break;
                case Enemy:
                    cout << "E ";
                    break;
            }
        }
        cout << endl;
    }
}

void Map::setPlayerPosition(Point p) {
    if (playerPosition->row != -1 || playerPosition->col != -1) {
        cells[playerPosition->row][playerPosition->col] = Corridor;
    }
    cells[p.row][p.col] = Player;
    delete(playerPosition);
    playerPosition = new Point(p);
}

void Map::setEnemyPosition(Point p) {
    if (enemyPosition->row != -1 || enemyPosition->col != -1) {
        cells[enemyPosition->row][enemyPosition->col] = Corridor;
    }
    cells[p.row][p.col] = Enemy;
    delete(enemyPosition);
    enemyPosition = new Point(p);
}

Point Map::getPlayerPosition() const {
    return Point(playerPosition->row, playerPosition->col);
}

Point Map::getEnemyPosition() const {
    return Point(enemyPosition->row, enemyPosition->col);
}

void Map::playerMove(Direction d) {
    Point neighborPoint = getNeighborPoint(getPlayerPosition(), d);
    CellType neighborCellType = getPointCellType(neighborPoint);
    switch (neighborCellType) {
        case Wall:
            break;
        case Food:
            eatFood(neighborPoint, Player);
        case Corridor:
            setPlayerPosition(neighborPoint);
            break;
        case Enemy:
            setPlayerPosition(playerInitialPosition);
            currentPlayerDirection = None;
            nextPlayerDirection = None;
            break;
        default:
            break;
    }
}

void Map::enemyMove(Direction d) {
    Point neighborPoint = getNeighborPoint(getEnemyPosition(), d);
    CellType neighborCellType = getPointCellType(neighborPoint);
    switch (neighborCellType) {
        case Wall:
            break;
        case Food:
            eatFood(neighborPoint, Enemy);
        case Corridor:
            setEnemyPosition(neighborPoint);
            break;
        case Player:
            currentPlayerDirection = None;
            nextPlayerDirection = None;
            setEnemyPosition(neighborPoint);
            setPlayerPosition(playerInitialPosition);
            break;
        default:
            break;
    }
}

bool Map::playerCanMoveTo(Direction d) {
    Point neighborPoint = getNeighborPoint(getPlayerPosition(), d);
    return getPointCellType(neighborPoint) != Wall;
}

bool Map::enemyCanMoveTo(Direction d) {
    Point neighborPoint = getNeighborPoint(getEnemyPosition(), d);
    return getPointCellType(neighborPoint) != Wall;
}

void Map::eatFood(Point p, CellType player) {
    if (cells[p.row][p.col] == Food) {
        cells[p.row][p.col] = Corridor;
        numberOfAvailableFood -= 1;
        foodCells.erase(p);
        if (player == Player) {
            eatedFoodByPlayer += 1;
        } else if (player == Enemy) {
            eatedFoodByEnemy += 1;
        }
    }
}

CellType Map::getPointCellType(Point p) {
    return cells[p.row][p.col];
}

Point Map::getNeighborPoint(Point p, Direction d) {
    switch (d) {
        case Up:
            return Point(p.row-1, p.col);
        case Down:
            return Point(p.row+1, p.col);
        case Left:
            return Point(p.row, p.col-1);
        case Right:
            return Point(p.row, p.col+1);
        default:
            return p;
    }
}

int Map::getEatedFoodByPlayer() {
    return eatedFoodByPlayer;
}
int Map::getEatedFoodByEnemy() {
    return eatedFoodByEnemy;
}

void Map::initGame() {
    setPlayerPosition(playerInitialPosition);
    setEnemyPosition(enemyInitialPosition);
    initializeAvailableFood();
    currentPlayerDirection = None;
    nextPlayerDirection = None;
}

void Map::initializeAvailableFood() {
    int availableFood = 0;
    foodCells.clear();
    for (int row = 0; row < nRows; row += 1) {
        for (int col = 0; col < nCols; col += 1) {
            if (cells[row][col] == Food) {
                availableFood += 1;
                foodCells.insert(Point(row, col));
            }
        }
    }
    numberOfAvailableFood = availableFood;
}

Direction Map::getCurrentPlayerDirection() {
    return currentPlayerDirection;
}

Direction Map::getNextPlayerDirection() {
    return nextPlayerDirection;
}

void Map::setCurrentPlayerDirection(Direction d) {
    currentPlayerDirection = d;
}
void Map::setNextPlayerDirection(Direction d) {
    nextPlayerDirection = d;
}

list<Direction> Map::getEnemyLegalMoves() {
    list<Direction> legalMoves;
    if (enemyCanMoveTo(Up)) legalMoves.push_back(Up);
    if (enemyCanMoveTo(Down)) legalMoves.push_back(Down);
    if (enemyCanMoveTo(Left)) legalMoves.push_back(Left);
    if (enemyCanMoveTo(Right)) legalMoves.push_back(Right);
    return legalMoves;
}

list<Direction> Map::getPlayerLegalMoves() {
    list<Direction> legalMoves;
    if (playerCanMoveTo(Up)) legalMoves.push_back(Up);
    if (playerCanMoveTo(Down)) legalMoves.push_back(Down);
    if (playerCanMoveTo(Left)) legalMoves.push_back(Left);
    if (playerCanMoveTo(Right)) legalMoves.push_back(Right);
    return legalMoves;
}

list<Direction> Map::getLegalMoves(CellType agent) {
    if (agent == Enemy) return getEnemyLegalMoves();
    else return getPlayerLegalMoves();
}

bool Map::isFoodAvailable() {
    return numberOfAvailableFood > 0;
}

Map Map::generateSuccessor(CellType agent, Direction action) {
    Map successor = Map(*this);
    if (agent == Player) {
        successor.playerMove(action);
    } else if (agent == Enemy) {
        successor.enemyMove(action);
    }
    return successor;
}

Map::Map(const Map &m) : nRows(m.nRows), nCols(m.nCols),
        eatedFoodByPlayer(m.eatedFoodByPlayer),
        eatedFoodByEnemy(m.eatedFoodByEnemy),
        playerInitialPosition(m.playerInitialPosition),
        enemyInitialPosition(m.enemyInitialPosition) {
    cells.assign(nRows, vector<CellType>(nCols, Wall));
    for (int i = 0; i < nRows; i += 1) {
        for (int j = 0; j < nCols; j += 1) {
            cells[i][j] = m.cells[i][j];
        }
    }
    playerPosition = new Point(m.getPlayerPosition());
    enemyPosition = new Point(m.getEnemyPosition());
    initializeAvailableFood();
    currentPlayerDirection = None;
    nextPlayerDirection = None;
}

set<Point> Map::getFoodCells() {
    return foodCells;
}
