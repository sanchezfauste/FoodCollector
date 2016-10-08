#include "map.h"
#include "mapgenerator.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        cout << "Use: " << argv[0] << "<nRows> <nCols>" << endl;
        return 1;
    }
    Map map = MapGenerator(atoi(argv[1]), atoi(argv[2])).getMap();
    map.print();
    return 0;
}