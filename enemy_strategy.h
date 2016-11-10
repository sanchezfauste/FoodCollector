/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __ENEMY_H
#define __ENEMY_H

#include "map.h"

using namespace std;

class EnemyStrategy {

    Map *map;

  public:
    EnemyStrategy(Map *map);
    Direction getAction();

};

#endif
