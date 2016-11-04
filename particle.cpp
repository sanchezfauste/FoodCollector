/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "particle.h"

using namespace std;

Particle::Particle() : state(Quiet) {}

void Particle::initMovement(float widthTranslation, float heightTranslation,
        long duration, Direction direction) {
    this->widthTranslation = widthTranslation;
    this->heightTranslation = heightTranslation;
    widthTranslationVector = widthTranslation / duration;
    heightTranslationVector = heightTranslation / duration;
    widthCurrentTranslation = 0;
    heightCurrentTranslation = 0;
    timeRemaining = duration;
    state = Moving;
    this->direction = direction;
}

bool Particle::integrate(long elapsedTime) {
    if (state == Moving && elapsedTime < timeRemaining) {
        widthCurrentTranslation += widthTranslationVector * elapsedTime;
        heightCurrentTranslation += heightTranslationVector * elapsedTime;
        timeRemaining -= elapsedTime;
    } else if (state == Moving && elapsedTime >= timeRemaining) {
        widthCurrentTranslation += widthTranslationVector * timeRemaining;
        heightCurrentTranslation += heightTranslationVector * timeRemaining;
        state = Quiet;
        return true;
    }
    return false;
}

float Particle::getCurrentWidthTranslation() {
    return widthCurrentTranslation;
}

float Particle::getCurrentHeightTranslation() {
    return heightCurrentTranslation;
}

State Particle::getState() {
    return state;
}

Direction Particle::getDirection() {
    return direction;
}
