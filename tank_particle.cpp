/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "tank_particle.h"
#include <cmath>

using namespace std;

const long TankParticle::tankRotationTime = 100;
const long TankParticle::tankMovementTime = 150;

TankParticle::TankParticle(Direction tankOrientation) : state(Quiet) {
    setTankOrientation(tankOrientation);
}

void TankParticle::initMovement(float widthTranslation, float heightTranslation,
        Direction direction, double speedFactor) {
    this->widthTranslation = widthTranslation;
    this->heightTranslation = heightTranslation;
    movementTimeRemaining = TankParticle::tankMovementTime * speedFactor;
    widthTranslationVector = widthTranslation / movementTimeRemaining;
    heightTranslationVector = heightTranslation / movementTimeRemaining;
    widthCurrentTranslation = 0;
    heightCurrentTranslation = 0;
    degreesCurrentTranslation = 0;
    state = Moving;
    this->direction = direction;
    if (tankOrientation != direction) {
        rotating = true;
        int rotationSteps = getTankRotationSteps(direction);
        rotationTimeRemaining =
            TankParticle::tankRotationTime * speedFactor * abs(rotationSteps);
        degreesTranslation = rotationSteps * 90.0;
        degreesTranslationVector = degreesTranslation / rotationTimeRemaining;
    } else {
        rotating = false;
        degreesTranslation = 0;
    }
}

int TankParticle::getTankRotationSteps(Direction d) {
    int steps = d - tankOrientation;
    if (steps == -3) steps = 1;
    if (steps == 3) steps = -1;
    return steps;
}

bool TankParticle::integrate(long elapsedTime) {
    if (rotating) {
        rotating = !integrateRotation(elapsedTime);
        return false;
    } else {
        return integrateMovement(elapsedTime);
    }
}

bool TankParticle::integrateMovement(long elapsedTime) {
    if (state == Moving && elapsedTime < movementTimeRemaining) {
        widthCurrentTranslation += widthTranslationVector * elapsedTime;
        heightCurrentTranslation += heightTranslationVector * elapsedTime;
        movementTimeRemaining -= elapsedTime;
    } else if (state == Moving && elapsedTime >= movementTimeRemaining) {
        widthCurrentTranslation += widthTranslationVector * movementTimeRemaining;
        heightCurrentTranslation += heightTranslationVector * movementTimeRemaining;
        state = Quiet;
        return true;
    }
    return false;
}

bool TankParticle::integrateRotation(long elapsedTime) {
    if (state == Moving && elapsedTime < rotationTimeRemaining) {
        degreesCurrentTranslation += degreesTranslationVector * elapsedTime;
        rotationTimeRemaining-= elapsedTime;
    } else if (state == Moving && elapsedTime >= rotationTimeRemaining) {
        setTankOrientation(direction);
        return true;
    }
    return false;
}

float TankParticle::getCurrentWidthTranslation() {
    return widthCurrentTranslation;
}

float TankParticle::getCurrentHeightTranslation() {
    return heightCurrentTranslation;
}

float TankParticle::getDegreesTranslation() {
    return tankRotationDegrees + (rotating ? degreesCurrentTranslation : 0);
}

State TankParticle::getState() {
    return state;
}

void TankParticle::setState(State state) {
    this->state = state;
}

Direction TankParticle::getDirection() {
    return direction;
}

void TankParticle::setTankOrientation(Direction orientation) {
    tankRotationDegrees = orientation * 90;
    tankOrientation = orientation;
}

Direction TankParticle::getTankOrientation() {
    return tankOrientation;
}

bool TankParticle::isRotating() {
    return rotating;
}
