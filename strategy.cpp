/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "strategy.h"
#include <stdlib.h>
#include <ctime>
#include <math.h>

Strategy::Strategy(Map *map) : map(map) {
    srand(time(NULL));
}

Strategy::~Strategy() {}

Direction Strategy::getRandomDirection(list<Direction> directions) {
    list<Direction>::iterator n = directions.begin();
    for (int i = 0, nrand = random() % directions.size(); i < nrand; i += 1, ++n) {}
    return *n;
}

double Strategy::manhattanDistance(Position p1, Position p2) {
    return fabs(p1.row - p2.row) + fabs(p1.col - p2.col);
}
