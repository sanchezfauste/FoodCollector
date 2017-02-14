/* 
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#ifndef __EXPECTIMAX_STRATEGY_PLAYER_H
#define __EXPECTIMAX_STRATEGY_PLAYER_H

#include "expectimax_strategy.h"

using namespace std;

class ExpectimaxStrategyPlayer : public ExpectimaxStrategy {

    double max_value(Map map, CellType agent, int depth);
    double min_value(Map map, CellType agent, int depth);
    Direction minimax_decision(int depth);
    double evaluationFunction(Map &map);

  public:
    ExpectimaxStrategyPlayer(Map *map);

};

#endif
