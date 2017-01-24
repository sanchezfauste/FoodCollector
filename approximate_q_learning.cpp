/*
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#include <string>
#include <sstream>
#include <cstdlib>
#include "approximate_q_learning.h"

QValuesKey::QValuesKey(const Map state, const Direction action) : state(state),
        action(action) {}

bool QValuesKey::operator<(const QValuesKey &qvk) const {
    ostringstream convert;
    ostringstream convert1;
    convert << state.toString() << ',' << action;
    convert1 << qvk.state.toString() << ',' << qvk.action;
    return convert.str() < convert1.str();
}

ApproximateQLearning::ApproximateQLearning(Map *map, const double epsilon,
        const double alpha, const double discount) : Strategy(map),
        epsilon(epsilon), alpha(alpha), discount(discount) {}

Direction ApproximateQLearning::getAction() {
    list<Direction> legalMoves = map->getLegalMoves(Enemy);
    Direction action = None;
    if (legalMoves.size() != 0) {
        if (flipCoin(epsilon)) {
            action = getRandomDirection(legalMoves);
        } else {
            action = getPolicy(*map);
        }
    }
    return action;
}

double ApproximateQLearning::getQValue(QValuesKey key) {
    double qValue = 0.0;
    std::map<string, double> features = getFeatures(key);
    for (std::map<string, double>::iterator a = features.begin(); a != features.end(); ++a) {
        qValue += /*weights[a->first] **/ a->second;
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
        if (abs(bestValue - getQValue(QValuesKey(map, *a))) < 0.0000001) {
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
    features["#-of-ghosts-1-step-away"] = manhattanDistance(
            key.state.getPlayerPosition(), key.state.getEnemyPosition()) == 1 ? 1 : 0;
    if (features["#-of-ghosts-1-step-away"] == 0
            && key.state.getPositionCellType(nextEnemyPosition) == Food) {
        features["eats-food"] = 10.0;
    }
    double dist = key.state.getClosestFoodDistance(nextEnemyPosition);
    if (dist > 0) {
        features["closest-food"] = dist / key.state.getDimension();
    }
    return features;
}

double ApproximateQLearning::manhattanDistance(Position p1, Position p2) {
    return Strategy::abs(p1.row - p2.row) + Strategy::abs(p1.col - p2.col);
}
