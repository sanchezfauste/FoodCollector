/*
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#ifndef __APPROXIMATE_Q_LEARNING_H
#define __APPROXIMATE_Q_LEARNING_H

#include <map>
#include "map.h"
#include "strategy.h"
#include "reinforcement_agent.h"

using namespace std;

class ApproximateQLearning : public ReinforcementAgent {

    std::map<QValuesKey, double> qvalues;
    std::map<string, double> weights;

    double getQValue(QValuesKey key);
    void setQValue(QValuesKey &key, double newValue);
    double computeValueFromQValues(Map &map);
    Direction computeActionFromQValues(Map &map);
    void update(QValuesKey &key, Map &nextState, double reward);
    Direction getPolicy(Map &map);
    double getValue(Map &map);
    bool flipCoin(double p);
    std::map<string, double> getFeatures(QValuesKey &key);
    double manhattanDistance(Position p1, Position p2);

  public:
    ApproximateQLearning(Map *map, const double epsilon = 0.05,
            const double alpha = 0.2, const double discount = 0.8);
    Direction getAction();

};

#endif
