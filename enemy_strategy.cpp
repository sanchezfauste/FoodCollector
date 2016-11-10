/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "enemy_strategy.h"
#include <stdlib.h>

EnemyStrategy::EnemyStrategy(Map *map) : map(map) {}

Direction EnemyStrategy::getAction() {
    list<Direction> directions = map->getEnemyLegalMoves();
    int rando = random() % directions.size();
    list<Direction>::iterator n = directions.begin();
    for (int i = 0; i < rando; i++, ++n) {}
    return *n;
}
