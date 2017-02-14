/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __TANK_PARTICLE_H
#define __TANK_PARTICLE_H

#include "map.h"
#include "particle.h"

using namespace std;

class TankParticle {

    float widthTranslation;
    float heightTranslation;
    float degreesTranslation;
    float widthTranslationVector;
    float heightTranslationVector;
    float degreesTranslationVector;
    float widthCurrentTranslation;
    float heightCurrentTranslation;
    float degreesCurrentTranslation;
    float tankRotationDegrees;
    long movementTimeRemaining;
    long rotationTimeRemaining;
    State state;
    Direction direction;
    Direction tankOrientation;
    bool rotating;

    bool integrateMovement(long elapsedTime);
    bool integrateRotation(long elapsedTime);
    int getTankRotationSteps(Direction d);

  public:
    static const long tankRotationTime; //Time to rotate 90 degrees
    static const long tankMovementTime; //Time to move one position

    TankParticle(Direction tankOrientation);
    void initMovement(float widthTranslation, float heightTranslation,
        Direction direction, double speedFactor);
    bool integrate(long elapsedTime);
    float getCurrentWidthTranslation();
    float getCurrentHeightTranslation();
    float getDegreesTranslation();
    State getState();
    void setState(State state);
    Direction getDirection();
    void setTankOrientation(Direction orientation);
    Direction getTankOrientation();
    bool isRotating();

};

#endif
