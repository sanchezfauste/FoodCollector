/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __ENEMY_H
#define __ENEMY_H

#include "map.h"

using namespace std;

class EnemyStrategy {

    Map *map;

    double utility(Map &map);
    bool terminalTest(Map &map, int depth);
    Map result(Map &map, CellType agent, Direction action);
    double max_value(Map map, CellType agent, int depth);
    double min_value(Map map, CellType agent, int depth);
    Direction minimax_decision(int depth);
    double evaluationFunction(Map &map);
    double max(double v1, double v2);
    double min(double v1, double v2);
    double abs(double v1);
    Direction getRandomDirection(list<Direction> directions);
    double minDistance(Point p1, Point p2);

  public:
    EnemyStrategy(Map *map);
    Direction getAction();

};

#endif
