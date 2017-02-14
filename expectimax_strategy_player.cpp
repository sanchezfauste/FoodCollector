/*
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#include <math.h>
#include "expectimax_strategy_player.h"

ExpectimaxStrategyPlayer::ExpectimaxStrategyPlayer(Map *map) : ExpectimaxStrategy(map) {}

double ExpectimaxStrategyPlayer::max_value(Map map, CellType agent, int depth) {
    if (terminalTest(map, depth)) {
        return utility(map);
    }
    list<Direction> legalMoves = map.getLegalMoves(agent);
    double v = -30000000;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        v = fmax(v, min_value(result(map, agent, *a), Enemy, depth));
    }
    return v;
}

double ExpectimaxStrategyPlayer::min_value(Map map, CellType agent, int depth) {
    if (terminalTest(map, depth)) {
        return utility(map);
    }
    list<Direction> legalMoves = map.getLegalMoves(agent);
    double v = 0;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        if (agent == Enemy) {
            v += max_value(result(map, agent, *a), Player, depth-1);
        } else {
            v += min_value(result(map, agent, *a), Enemy, depth);
        }
    }
    return v/legalMoves.size();
}

Direction ExpectimaxStrategyPlayer::minimax_decision(int depth) {
    double v = -3000000;
    list<Direction> legalMoves = map->getLegalMoves(Player);
    list<Direction> moves;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        double u = min_value(result(*map, Player, *a), Enemy, depth);
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

double ExpectimaxStrategyPlayer::evaluationFunction(Map &map) {

    double totalScore = map.getEatedFoodByPlayer() * 10;
    Position playerPosition = map.getPlayerPosition();
    set<Position> foodCells = map.getFoodCells();
    for (set<Position>::iterator a = foodCells.begin(); a != foodCells.end(); ++a) {
        double d = minDistance(*a, playerPosition);
        totalScore += 1.0/(d*d);
    }
    return totalScore;
}
