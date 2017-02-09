/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include <math.h>
#include "expectimax_strategy.h"

ExpectimaxStrategy::ExpectimaxStrategy(Map *map) : Strategy(map) {}

Direction ExpectimaxStrategy::getAction() {
    return minimax_decision(2);
}

double ExpectimaxStrategy::utility(Map &map) {
    return evaluationFunction(map);
}

bool ExpectimaxStrategy::terminalTest(Map &map, int depth) {
    return depth == 0 || !map.isFoodAvailable();
}

Map ExpectimaxStrategy::result(Map &map, CellType agent, Direction action) {
    return map.generateSuccessor(agent, action);
}

double ExpectimaxStrategy::max_value(Map map, CellType agent, int depth) {
    if (terminalTest(map, depth)) {
        return utility(map);
    }
    list<Direction> legalMoves = map.getLegalMoves(agent);
    double v = -30000000;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        v = fmax(v, min_value(result(map, agent, *a), Player, depth));
    }
    return v;
}

double ExpectimaxStrategy::min_value(Map map, CellType agent, int depth) {
    if (terminalTest(map, depth)) {
        return utility(map);
    }
    list<Direction> legalMoves = map.getLegalMoves(agent);
    double v = 0;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        if (agent == Player) {
            v += max_value(result(map, agent, *a), Enemy, depth-1);
        } else {
            v += min_value(result(map, agent, *a), Player, depth);
        }
    }
    return v/legalMoves.size();
}

Direction ExpectimaxStrategy::minimax_decision(int depth) {
    double v = -3000000;
    list<Direction> legalMoves = map->getLegalMoves(Enemy);
    list<Direction> moves;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        double u = min_value(result(*map, Enemy, *a), Player, depth);
        if (u == v) {
            moves.push_back(*a);
        } else if (u > v) {
            v = u;
            moves.clear();
            moves.push_back(*a);
        }
    }
    return getRandomDirection(moves);
}

double ExpectimaxStrategy::evaluationFunction(Map &map) {
    double totalScore = map.getEatedFoodByEnemy() * 10;
    Position enemyPosition = map.getEnemyPosition();
    set<Position> foodCells = map.getFoodCells();
    for (set<Position>::iterator a = foodCells.begin(); a != foodCells.end(); ++a) {
        double d = minDistance(*a, enemyPosition);
        totalScore += 1.0/(d*d);
    }
    return totalScore;
}

double ExpectimaxStrategy::minDistance(Position p1, Position p2) {
    return Strategy::manhattanDistance(p1, p2);
}
