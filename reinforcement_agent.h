/*
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#ifndef __REINFORCEMENT_AGENT_H
#define __REINFORCEMENT_AGENT_H

#include <map>
#include "map.h"
#include "strategy.h"

typedef struct QValuesKey {
    const Map state;
    const Direction action;
    QValuesKey(const Map state, const Direction action);
    bool operator<(const QValuesKey &qvk) const;
} QValuesKey;

class ReinforcementAgent : public Strategy {

    int numTraining;
    int episodes;
    double accumTrainRewards;
    double accumTestRewards;
    Map *lastState;
    Direction lastAction;
    double episodeRewards;

    virtual void update(QValuesKey &key, Map &nextState, double reward) = 0;
    void observeTransition(QValuesKey key, Map &nextState, double deltaReward);
    void startEpisode();
    void stopEpisode();

  protected:
    double epsilon;
    double alpha;
    double discount;

    void doAction(Map &state, Direction action);
    void observationFunction(Map &state);

  public:
    ReinforcementAgent(Map *map, int numTraining, double epsilon, double alpha,
            double gamma);
    void final();
    void registerInitialState(Map *state);

};

#endif
