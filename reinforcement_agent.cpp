/*
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#include <string>
#include <sstream>
#include <iostream>
#include "reinforcement_agent.h"

QValuesKey::QValuesKey(const Map state, const Direction action) : state(state),
        action(action) {}

bool QValuesKey::operator<(const QValuesKey &qvk) const {
    ostringstream convert;
    ostringstream convert1;
    convert << state.toString() << ',' << action;
    convert1 << qvk.state.toString() << ',' << qvk.action;
    return convert.str() < convert1.str();
}

ReinforcementAgent::ReinforcementAgent(Map *map, int numTraining, double epsilon,
        double alpha, double gamma) : Strategy(map), numTraining(numTraining),
        episodes(0), accumTrainRewards(0.0), accumTestRewards(0.0),
        epsilon(epsilon), alpha(alpha), discount(gamma) {}

void ReinforcementAgent::observeTransition(QValuesKey key, Map &nextState,
        double deltaReward) {
    episodeRewards += deltaReward;
    update(key, nextState, deltaReward);
}

void ReinforcementAgent::startEpisode() {
    lastState = NULL;
    lastAction = None;
    episodeRewards = 0.0;
}

void ReinforcementAgent::stopEpisode() {
    if (episodes < numTraining) {
        accumTrainRewards += episodeRewards;
    } else {
        accumTestRewards += episodeRewards;
    }
    episodes += 1;
    if (episodes >= numTraining) {
        epsilon = 0.0;
        alpha = 0.0;
    }
}

void ReinforcementAgent::observationFunction(Map &state) {
    if (lastState != NULL) {
        double reward = state.getScore() - lastState->getScore();
        observeTransition(QValuesKey(*lastState, lastAction), state, reward);
    }
}

void ReinforcementAgent::registerInitialState(Map *state) {
    map = state;
    startEpisode();
    if (episodes == 0) {
        cout << "Begining " << numTraining << " episodes of training" << endl;
    }
}

void ReinforcementAgent::final() {
    //Called by Pacman game at the terminal state
    double deltaReward = map->getScore() - lastState->getScore();
    observeTransition(QValuesKey(*lastState, lastAction), *map, deltaReward);
    stopEpisode();
    double lastWindowAccumRewards = map->getScore();
    int NUM_EPS_UPDATE = 25;
    if (episodes % NUM_EPS_UPDATE == 0) {
        cout << "Reinforcement Learning Status:" << endl;
        double windowAvg = lastWindowAccumRewards / NUM_EPS_UPDATE;
        if (episodes <= numTraining) {
            double trainAvg = accumTrainRewards / episodes;
            cout << "\tCompleted " << episodes << " out of "
                << numTraining << " training episodes" << endl;
            cout << "\tAverage Rewards over all training: " << trainAvg << endl;
        } else {
            double testAvg = accumTestRewards / (episodes - numTraining);
            cout << "\tCompleted " << episodes - numTraining
                << " test episodes" << endl;
            cout << "\tAverage Rewards over testing: " << testAvg << endl;
        }
        cout << "\tAverage Rewards for last " << NUM_EPS_UPDATE
            << " episodes: " << windowAvg << endl;
    }
    if (episodes == numTraining) {
        cout << "Training Done (turning off epsilon and alpha)" << endl;
    }
}

void ReinforcementAgent::doAction(Map &state, Direction action) {
    if (lastState != NULL) delete(lastState);
    lastState = new Map(state);
    lastAction = action;
}
