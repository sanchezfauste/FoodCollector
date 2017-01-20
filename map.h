/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __MAP_H
#define __MAP_H

#include <vector>
#include <set>
#include <list>
#include <string>

using namespace std;

enum CellType {
    Wall,
    Corridor,
    Food,
    Player,
    Enemy
};

enum Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3,
    None = 4
};

typedef struct Position {
    const int row;
    const int col;
    Position(const int row, const int col);
    Position();
    bool operator<(const Position &p) const;
    bool operator==(const Position &p) const;
} Position;

class Map {

    vector<vector<CellType> > cells;
    const int nRows, nCols;
    Position *playerPosition;
    Position *enemyPosition;
    int eatedFoodByPlayer;
    int eatedFoodByEnemy;
    const Position playerInitialPosition;
    const Position enemyInitialPosition;
    int numberOfAvailableFood;
    Direction currentPlayerDirection;
    Direction nextPlayerDirection;
    set<Position> foodCells;

    void initializeAvailableFood();
    void eatFood(Position p, CellType player);
    list<Direction> getEnemyLegalMoves();
    list<Direction> getPlayerLegalMoves();

  public:
    static const int minRows;
    static const int minCols;

    Map(const int nRows, const int nCols);
    Map(const int nRows, const int nCols, const Position playerInitialPosition,
            const Position enemyInitialPosition);
    Map(const Map &m);
    void print();
    void copySymmetricLeftToRight();
    int getNumberOfRows();
    int getNumberOfCols();
    void copySubMap(Map &m, Position startingPosition);
    void setAreaCellsType(Position &initialPosition, Position &finalPosition, CellType ct);
    void setSubColCellType(int col, int rowBegin, int rowEnd, CellType ct);
    bool allCellsVisited(set<Position> &visited);
    list<Position> getNeighbors(Position &p);
    list<Position> getUnvisitedNeighbors(Position &p, set<Position> &visitedCells);
    Position getRandomPositionOfList(list<Position> &Positions);
    void removeWall(Position &p, Position &neighbor);
    vector<CellType> getRow(int row);
    void setPlayerPosition(Position p);
    void setEnemyPosition(Position p);
    Position getPlayerPosition() const;
    Position getEnemyPosition() const;
    void playerMove(Direction d);
    void enemyMove(Direction d);
    bool playerCanMoveTo(Direction d);
    bool enemyCanMoveTo(Direction d);
    CellType getPositionCellType(Position p);
    Position getNeighborPosition(Position p, Direction d);
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
    set<Position> getFoodCells();
    void shootEnemy();
    string toString() const;

};

#endif
