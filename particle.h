/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __PARTICLE_H
#define __PARTICLE_H

#include "map.h"

using namespace std;

enum State {
    Moving,
    Quiet
};

class Particle {

    float widthTranslation;
    float heightTranslation;
    float widthTranslationVector;
    float heightTranslationVector;
    float widthCurrentTranslation;
    float heightCurrentTranslation;
    long timeRemaining;
    State state;
    Direction direction;

  public:
    Particle();
    void initMovement(float widthTranslation, float heightTranslation,
            long duration, Direction direction);
    bool integrate(long elapsedTime);
    float getCurrentWidthTranslation();
    float getCurrentHeightTranslation();
    State getState();
    void setState(State state);
    Direction getDirection();

};

#endif
