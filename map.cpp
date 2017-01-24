/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "map.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stack>
#include <set>

using namespace std;

const int Map::minRows = 5;
const int Map::minCols = 5;

Position::Position(const int row, const int col) : row(row), col(col){}

Position::Position() : row(-1), col(-1){}

FringeElement::FringeElement(const Position pos, const int distance) :
        pos(pos), distance(distance) {}

bool Position::operator<(const Position &p) const {
    return (row == p.row) ? col < p.col : row < p.row;
}

bool Position::operator==(const Position &p) const {
    return row == p.row && col == p.col;
}

Map::Map(const int nRows, const int nCols) : nRows(nRows), nCols(nCols),
        eatedFoodByPlayer(0), eatedFoodByEnemy(0),
        playerInitialPosition(Position(1, 1)),
        enemyInitialPosition(Position(1, nCols - 2)) {
    cells.assign(nRows, vector<CellType>(nCols, Wall));
    playerPosition = new Position();
    enemyPosition = new Position();
}

Map::Map(const int nRows, const int nCols, const Position playerInitialPosition,
        const Position enemyInitialPosition) : nRows(nRows), nCols(nCols),
                eatedFoodByPlayer(0), eatedFoodByEnemy(0),
                playerInitialPosition(playerInitialPosition),
                enemyInitialPosition(enemyInitialPosition) {
    cells.assign(nRows, vector<CellType>(nCols, Wall));
    playerPosition = new Position();
    enemyPosition = new Position();
}

bool Map::allCellsVisited(set<Position> &visited) {
    static const unsigned int visitable_cells =
            (nRows-1) / 2 * (nCols-1) / 2;
    return visited.size() == visitable_cells;
}

void Map::copySubMap(Map &m, Position startingPosition) {
    for (unsigned int i = 0; i < m.cells.size(); i += 1) {
        for (unsigned int j = 0; j < m.cells[i].size(); j += 1) {
            cells[startingPosition.row+i][startingPosition.col+j] = m.cells[i][j];
        }
    }
}

list<Position> Map::getNeighbors(Position &p) {
    list<Position> neighbors;
    if (p.col-2 >= 0) neighbors.push_back(Position(p.row, p.col-2));
    if (p.col+2 < nCols) neighbors.push_back(Position(p.row, p.col+2));
    if (p.row-2 >= 0) neighbors.push_back(Position(p.row-2, p.col));
    if (p.row+2 < nRows) neighbors.push_back(Position(p.row+2, p.col));
    return neighbors;
}

void Map::removeWall(Position &p, Position &neighbor) {
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

list<Position> Map::getUnvisitedNeighbors(Position &p, set<Position> &visitedCells) {
    list<Position> unvisitedNeighbors;
    list<Position> neighbors = getNeighbors(p);
    for (list<Position>::iterator n = neighbors.begin(); n != neighbors.end(); ++n) {
        if (visitedCells.count(*n) == 0) unvisitedNeighbors.push_back(*n);
    }
    return unvisitedNeighbors;
}

Position Map::getRandomPositionOfList(list<Position> &Positions) {
    int randomElement = rand() % Positions.size();
    list<Position>::iterator p = Positions.begin();
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

void Map::setAreaCellsType(Position &initialPosition, Position &finalPosition, CellType ct) {
    for (int row = initialPosition.row; row < finalPosition.row; row += 1) {
        for (int col = initialPosition.col; col < finalPosition.col; col += 1) {
            cells[row][col] = ct;
        }
    }
}

vector<CellType> Map::getRow(int row) {
    return cells[row];
}

void Map::print() const {
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

void Map::setPlayerPosition(Position p) {
    if (playerPosition->row != -1 || playerPosition->col != -1) {
        cells[playerPosition->row][playerPosition->col] = Corridor;
    }
    cells[p.row][p.col] = Player;
    delete(playerPosition);
    playerPosition = new Position(p);
}

void Map::setEnemyPosition(Position p) {
    if (enemyPosition->row != -1 || enemyPosition->col != -1) {
        cells[enemyPosition->row][enemyPosition->col] = Corridor;
    }
    cells[p.row][p.col] = Enemy;
    delete(enemyPosition);
    enemyPosition = new Position(p);
}

Position Map::getPlayerPosition() const {
    return Position(playerPosition->row, playerPosition->col);
}

Position Map::getEnemyPosition() const {
    return Position(enemyPosition->row, enemyPosition->col);
}

void Map::playerMove(Direction d) {
    Position neighborPosition = getNeighborPosition(getPlayerPosition(), d);
    CellType neighborCellType = getPositionCellType(neighborPosition);
    switch (neighborCellType) {
        case Wall:
            break;
        case Food:
            eatFood(neighborPosition, Player);
        case Corridor:
            setPlayerPosition(neighborPosition);
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
    Position neighborPosition = getNeighborPosition(getEnemyPosition(), d);
    CellType neighborCellType = getPositionCellType(neighborPosition);
    switch (neighborCellType) {
        case Wall:
            break;
        case Food:
            eatFood(neighborPosition, Enemy);
        case Corridor:
            setEnemyPosition(neighborPosition);
            break;
        case Player:
            currentPlayerDirection = None;
            nextPlayerDirection = None;
            setEnemyPosition(neighborPosition);
            setPlayerPosition(playerInitialPosition);
            break;
        default:
            break;
    }
}

bool Map::playerCanMoveTo(Direction d) const {
    Position neighborPosition = getNeighborPosition(getPlayerPosition(), d);
    return getPositionCellType(neighborPosition) != Wall;
}

bool Map::enemyCanMoveTo(Direction d) const {
    Position neighborPosition = getNeighborPosition(getEnemyPosition(), d);
    return getPositionCellType(neighborPosition) != Wall;
}

bool Map::canMoveTo(Position &p, Direction d) const {
    return getPositionCellType(getNeighborPosition(p, d)) != Wall;
}

void Map::eatFood(Position p, CellType player) {
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

CellType Map::getPositionCellType(Position p) const {
    return cells[p.row][p.col];
}

Position Map::getNeighborPosition(Position p, Direction d) const {
    switch (d) {
        case Up:
            return Position(p.row-1, p.col);
        case Down:
            return Position(p.row+1, p.col);
        case Left:
            return Position(p.row, p.col-1);
        case Right:
            return Position(p.row, p.col+1);
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
                foodCells.insert(Position(row, col));
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

list<Direction> Map::getEnemyLegalMoves() const {
    list<Direction> legalMoves;
    if (enemyCanMoveTo(Up)) legalMoves.push_back(Up);
    if (enemyCanMoveTo(Down)) legalMoves.push_back(Down);
    if (enemyCanMoveTo(Left)) legalMoves.push_back(Left);
    if (enemyCanMoveTo(Right)) legalMoves.push_back(Right);
    return legalMoves;
}

list<Direction> Map::getPlayerLegalMoves() const {
    list<Direction> legalMoves;
    if (playerCanMoveTo(Up)) legalMoves.push_back(Up);
    if (playerCanMoveTo(Down)) legalMoves.push_back(Down);
    if (playerCanMoveTo(Left)) legalMoves.push_back(Left);
    if (playerCanMoveTo(Right)) legalMoves.push_back(Right);
    return legalMoves;
}

list<Direction> Map::getLegalMoves(CellType agent) const {
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
    playerPosition = new Position(m.getPlayerPosition());
    enemyPosition = new Position(m.getEnemyPosition());
    initializeAvailableFood();
    currentPlayerDirection = None;
    nextPlayerDirection = None;
}

set<Position> Map::getFoodCells() const {
    return foodCells;
}

void Map::shootEnemy() {
    setEnemyPosition(enemyInitialPosition);
}

string Map::toString() const {
    ostringstream convert;
    for (int i = 0; i < nRows; i += 1) {
        for (int j = 0; j < nCols; j += 1) {
            convert << cells[i][j];
        }
    }
    return convert.str();
}

Position Map::getNextEnemyPosition(Direction d) const {
    return getNeighborPosition(*enemyPosition, d);
}

list<Position> Map::getLegalNeighbors(Position p) const {
    list<Position> legalNeighbors;
    if (canMoveTo(p, Up)) legalNeighbors.push_back(getNeighborPosition(p, Up));
    if (canMoveTo(p, Down)) legalNeighbors.push_back(getNeighborPosition(p, Down));
    if (canMoveTo(p, Left)) legalNeighbors.push_back(getNeighborPosition(p, Left));
    if (canMoveTo(p, Right)) legalNeighbors.push_back(getNeighborPosition(p, Right));
    return legalNeighbors;
}

double Map::getClosestFoodDistance(Position &p) const {
    set<Position> expanded;
    stack<FringeElement> fringe;
    fringe.push(FringeElement(p, 0));
    while(!fringe.empty()) {
        FringeElement elem = fringe.top();
        fringe.pop();
        if (expanded.find(elem.pos) != expanded.end()) continue;
        expanded.insert(elem.pos);
        if (getPositionCellType(elem.pos) == Food) return elem.distance;
        list<Position> legalNeighbors = getLegalNeighbors(elem.pos);
        for (list<Position>::iterator a = legalNeighbors.begin(); a != legalNeighbors.end(); ++a) {
            fringe.push(FringeElement(*a, elem.distance + 1));
        }
    }
    return -1;
}

long Map::getDimension() const {
    return nRows * nCols;
}
