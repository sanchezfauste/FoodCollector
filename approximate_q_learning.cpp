/*
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#include <cstdlib>
#include <math.h>
#include "approximate_q_learning.h"

ApproximateQLearning::ApproximateQLearning(Map *map, const double epsilon,
        const double alpha, const double discount) :
        ReinforcementAgent(map, 10, epsilon, alpha, discount) {
    registerInitialState(map);
}

Direction ApproximateQLearning::getAction() {
    observationFunction(*map);
    list<Direction> legalMoves = map->getLegalMoves(Enemy);
    Direction action = None;
    if (legalMoves.size() != 0) {
        if (flipCoin(epsilon)) {
            action = getRandomDirection(legalMoves);
        } else {
            action = getPolicy(*map);
        }
    }
    doAction(*map, action);
    return action;
}

double ApproximateQLearning::getQValue(QValuesKey key) {
    double qValue = 0.0;
    std::map<string, double> features = getFeatures(key);
    for (std::map<string, double>::iterator a = features.begin(); a != features.end(); ++a) {
        qValue += weights[a->first] * a->second;
    }
    return qValue;
}

void ApproximateQLearning::setQValue(QValuesKey &key, double newValue) {
    qvalues[key] = newValue;
}

double ApproximateQLearning::computeValueFromQValues(Map &map) {
    list<Direction> legalMoves = map.getLegalMoves(Enemy);
    if (legalMoves.size() == 0) return 0.0;
    double max = 0.0;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        double qValue = getQValue(QValuesKey(map, *a));
        if (qValue > max) max = qValue;
    }
    return max;
}

Direction ApproximateQLearning::computeActionFromQValues(Map &map) {
    list<Direction> legalMoves = map.getLegalMoves(Enemy);
    double bestValue = getValue(map);
    list<Direction> bestActions;
    for (list<Direction>::iterator a = legalMoves.begin(); a != legalMoves.end(); ++a) {
        if (fabs(bestValue - getQValue(QValuesKey(map, *a))) < 0.0000001) {
            bestActions.push_back(*a);
        }
    }
    return getRandomDirection(bestActions);
}

void ApproximateQLearning::update(QValuesKey &key, Map &nextState, double reward) {
    std::map<string, double> features = getFeatures(key);
    double difference = (reward + discount * getValue(nextState)) - getQValue(key);
    for (std::map<string, double>::iterator a = features.begin(); a != features.end(); ++a) {
        weights[a->first] += alpha * difference * a->second;
    }
}

Direction ApproximateQLearning::getPolicy(Map &map) {
    return computeActionFromQValues(map);
}

double ApproximateQLearning::getValue(Map &map) {
    return computeValueFromQValues(map);
}

bool ApproximateQLearning::flipCoin(double p) {
    return (rand() / 1.0) < p;
}

std::map<string, double> ApproximateQLearning::getFeatures(QValuesKey &key) {
    std::map<string, double> features;
    features["bias"] = 1.0;
    Position nextEnemyPosition = key.state.getNextEnemyPosition(key.action);
    if (Strategy::manhattanDistance(key.state.getPlayerPosition(),
            key.state.getEnemyPosition()) == 1) {
        features["#-of-ghosts-1-step-away"] = 1.0;
    } else {
        features["#-of-ghosts-1-step-away"] = 0.0;
        if (key.state.getPositionCellType(nextEnemyPosition) == Food) {
            features["eats-food"] = 1.0;
        }
    }
    int dist = key.state.getClosestFoodDistance(nextEnemyPosition);
    if (dist > 0) {
        features["closest-food"] = (double) dist / key.state.getDimension();
    }
    divideMapBy(features, 10.0);
    return features;
}

void ApproximateQLearning::divideMapBy(std::map<string, double> &features,
        double divisor) {
    for (std::map<string, double>::iterator a = features.begin(); a != features.end(); ++a) {
        features[a->first] = a->second / divisor;
    }
}
