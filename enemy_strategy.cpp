/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "enemy_strategy.h"
#include <stdlib.h>
#include <ctime>

EnemyStrategy::EnemyStrategy(Map *map) : map(map) {}

Direction EnemyStrategy::getAction() {
    return minimax_decision(2);
}

double EnemyStrategy::utility(Map &map) {
    return evaluationFunction(map);
}

bool EnemyStrategy::terminalTest(Map &map, int depth) {
    return depth == 0 || !map.isFoodAvailable();
}

Map EnemyStrategy::result(Map &map, CellType agent, Direction action) {
    return map.generateSuccessor(agent, action);
}

double EnemyStrategy::max_value(Map &map, CellType agent, int depth) {
    if (terminalTest(map, depth)) {
        return utility(map);
    }
    list<Direction> legalMoves = map.getLegalMoves(agent);
    double v = -30000000;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        v = max(v, min_value(result(map, agent, *a), Player, depth));
    }
    return v;
}

double EnemyStrategy::min_value(Map map, CellType agent, int depth) {
    if (terminalTest(map, depth)) {
        return utility(map);
    }
    list<Direction> legalMoves = map.getLegalMoves(agent);
    double v = +30000000;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        if (agent == Player) {
            v = min(v, min_value(result(map, agent, *a), Enemy, depth-1));
        } else {
            v = min(v, min_value(result(map, agent, *a), Player, depth));
        }
    }
    return v;
}

Direction EnemyStrategy::minimax_decision(int depth) {
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

double EnemyStrategy::evaluationFunction(Map &map) {
    double totalScore = map.getEatedFoodByEnemy() * 100;
    Point enemyPosition = map.getEnemyPosition();
    set<Point> foodCells = map.getFoodCells();
    for (set<Point>::iterator a = foodCells.begin(); a != foodCells.end(); ++a) {
        double d = minDistance(*a, enemyPosition);
        totalScore += (d == 0) ? 100 : 1.0/(d*d);
    }
    Point playerPosition = map.getPlayerPosition();
    double d = minDistance(enemyPosition, playerPosition);
    totalScore += (d <= 1) ? 300 : 0;
    return totalScore;
}

double EnemyStrategy::max(double v1, double v2) {
    return v1 > v2 ? v1 : v2;
}

double EnemyStrategy::min(double v1, double v2) {
    return v1 < v2 ? v1 : v2;
}

double EnemyStrategy::abs(double v1) {
    return (v1 < 0) ? (v1 * -1) : v1;
}

Direction EnemyStrategy::getRandomDirection(list<Direction> directions) {
    srand(time(NULL));
    list<Direction>::iterator n = directions.begin();
    for (int i = 0, nrand = random() % directions.size(); i < nrand; i += 1, ++n) {}
    return *n;
}

double EnemyStrategy::minDistance(Point p1, Point p2) {
    return abs(p1.row - p2.row) + abs(p1.col - p2.col);
}
