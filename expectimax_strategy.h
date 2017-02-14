/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __EXPECTIMAX_STRATEGY_H
#define __EXPECTIMAX_STRATEGY_H

#include "map.h"
#include "strategy.h"

using namespace std;

class ExpectimaxStrategy : public Strategy {

    virtual double max_value(Map map, CellType agent, int depth);
    virtual double min_value(Map map, CellType agent, int depth);
    virtual Direction minimax_decision(int depth);
    virtual double evaluationFunction(Map &map);

  protected:
    double utility(Map &map);
    bool terminalTest(Map &map, int depth);
    Map result(Map &map, CellType agent, Direction action);
    double minDistance(Position p1, Position p2);

  public:
    ExpectimaxStrategy(Map *map);
    Direction getAction();

};

#endif
