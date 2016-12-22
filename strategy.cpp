/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "strategy.h"
#include <stdlib.h>
#include <ctime>

Strategy::Strategy(Map *map) : map(map) {
    srand(time(NULL));
}

Strategy::~Strategy() {}

double Strategy::max(double v1, double v2) {
    return v1 > v2 ? v1 : v2;
}

double Strategy::min(double v1, double v2) {
    return v1 < v2 ? v1 : v2;
}

double Strategy::abs(double v1) {
    return (v1 < 0) ? (v1 * -1) : v1;
}

Direction Strategy::getRandomDirection(list<Direction> directions) {
    list<Direction>::iterator n = directions.begin();
    for (int i = 0, nrand = random() % directions.size(); i < nrand; i += 1, ++n) {}
    return *n;
}
