/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __STRATEGY_H
#define __STRATEGY_H

#include "map.h"

using namespace std;

class Strategy {

  protected:
    Map *map;
    Strategy(Map *map);

  public:
    virtual Direction getAction() = 0;
    virtual ~Strategy();

};

#endif
