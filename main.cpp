/* 
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "map.h"
#include "mapgenerator.h"
#include "graphic.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Use: " << argv[0] << " <nRows> <nCols>" << endl;
        return 1;
    }
    int graphicWidth = atoi(argv[2]) * Graphic::cellWidth;
    int graphicHeight = atoi(argv[1]) * Graphic::cellHeight;
    Graphic& g = Graphic::getInstance();
    g.glutInitialize(&argc, argv);
    if (graphicWidth > g.getScreenWidth() || graphicHeight > g.getScreenHeight()
            || atoi(argv[1]) < Map::minRows || atoi(argv[2]) < Map::minCols) {
        cout << "ERROR: Incorrect map dimmensions!" << endl;
        cout << "Use: " << argv[0] << " <nRows> <nCols>" << endl;
        cout << "    <nRows> must be between [" << Map::minRows << ", "
                << g.getScreenHeight() / Graphic::cellHeight << "]" << endl;
        cout << "    <nCols> must be between [" << Map::minCols << ", "
                << g.getScreenWidth() / Graphic::cellWidth << "]" << endl;
        exit(1);
    }
    g.setGlutDimensions(graphicWidth, graphicHeight);
    Map map = MapGenerator(atoi(argv[1]), atoi(argv[2])).getMap();
    map.print();
    g.setMap(map);
    g.glutRun();
    return 0;
}
