/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __MAP_GENERATOR_H
#define __MAP_GENERATOR_H

#include "map.h"

using namespace std;

class MapGenerator {

    Map map;
    void generate();
    int nRows;
    int nCols;

  public:
    MapGenerator(const int nRows, const int nCols);
    Map getMap();

};

#endif
