/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#ifndef __TEXTURE_LOADER_H
#define __TEXTURE_LOADER_H

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jpeglib.h"

using namespace std;

enum Texture {
    Water = 0,
    Grass = 1,
    Edge = 2
    #ifdef ARDUINO
        ,WaterPurple = 3,
        WaterBlue = 4,
        WaterYellow = 5,
        WaterOrange = 6,
        WaterRed = 7
    #endif
};

class TextureLoader {

    static const char* const waterTextureFile;
    static const char* const grassTextureFile;
    static const char* const edgeTextureFile;
    static const int textureDimension;

    #ifdef ARDUINO
        static const char* const waterPurpleTextureFile;
        static const char* const waterBlueTextureFile;
        static const char* const waterYellowTextureFile;
        static const char* const waterOrangeTextureFile;
        static const char* const waterRedTextureFile;
    #endif

    static void ReadJPEG(const char *filename, unsigned char **image, int *width,
          int *height);
    static void LoadTexture(const char *filename, int dim);

  public:
    static void LoadTextures();

};

#endif
