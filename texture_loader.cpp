/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include "texture_loader.h"

using namespace std;

const char* const TextureLoader::waterTextureFile = "textures/water.jpeg";
const char* const TextureLoader::grassTextureFile = "textures/grass.jpeg";
const char* const TextureLoader::edgeTextureFile = "textures/edge.jpeg";
const int TextureLoader::textureDimension = 64;

#ifdef ARDUINO
    const char* const TextureLoader::waterPurpleTextureFile = "textures/water_purple.jpeg";
    const char* const TextureLoader::waterBlueTextureFile = "textures/water_blue.jpeg";
    const char* const TextureLoader::waterYellowTextureFile = "textures/water_yellow.jpeg";
    const char* const TextureLoader::waterOrangeTextureFile = "textures/water_orange.jpeg";
    const char* const TextureLoader::waterRedTextureFile = "textures/water_red.jpeg";
#endif

void TextureLoader::LoadTextures() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Water);
    TextureLoader::LoadTexture(TextureLoader::waterTextureFile,
            TextureLoader::textureDimension);
    glBindTexture(GL_TEXTURE_2D, Grass);
    TextureLoader::LoadTexture(TextureLoader::grassTextureFile,
            TextureLoader::textureDimension);
    glBindTexture(GL_TEXTURE_2D, Edge);
    TextureLoader::LoadTexture(TextureLoader::edgeTextureFile,
            TextureLoader::textureDimension);
    #ifdef ARDUINO
        glBindTexture(GL_TEXTURE_2D, WaterPurple);
        TextureLoader::LoadTexture(TextureLoader::waterPurpleTextureFile,
            TextureLoader::textureDimension);
        glBindTexture(GL_TEXTURE_2D, WaterBlue);
        TextureLoader::LoadTexture(TextureLoader::waterBlueTextureFile,
            TextureLoader::textureDimension);
        glBindTexture(GL_TEXTURE_2D, WaterYellow);
        TextureLoader::LoadTexture(TextureLoader::waterYellowTextureFile,
            TextureLoader::textureDimension);
        glBindTexture(GL_TEXTURE_2D, WaterOrange);
        TextureLoader::LoadTexture(TextureLoader::waterOrangeTextureFile,
            TextureLoader::textureDimension);
        glBindTexture(GL_TEXTURE_2D, WaterRed);
        TextureLoader::LoadTexture(TextureLoader::waterRedTextureFile,
            TextureLoader::textureDimension);
    #endif
    glDisable(GL_TEXTURE_2D);
}

void TextureLoader::ReadJPEG(const char *filename, unsigned char **image,
        int *width, int *height) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * infile;
    unsigned char **buffer;
    unsigned int i, j;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);


    if ((infile = fopen(filename, "rb")) == NULL) {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_calc_output_dimensions(&cinfo);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height = cinfo.output_height;


    *image = (unsigned char*) malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);

    buffer = (unsigned char **) malloc(1 * sizeof (unsigned char **));
    buffer[0] = (unsigned char *) malloc(cinfo.output_width * cinfo.output_components);


    i = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        for (j = 0; j < cinfo.output_width * cinfo.output_components; j++) {
            (*image)[i] = buffer[0][j];
            i++;
        }

    }

    free(buffer);
    jpeg_finish_decompress(&cinfo);
}

void TextureLoader::LoadTexture(const char *filename, int dim) {
    unsigned char *buffer;
    unsigned char *buffer2;
    int width, height;
    long i, j;
    long k, h;

    ReadJPEG(filename, &buffer, &width, &height);

    buffer2 = (unsigned char*) malloc(dim * dim * 3);

    //-- The texture pattern is subsampled so that its dimensions become dim x dim --
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++) {
            k = i * height / dim;
            h = j * width / dim;

            buffer2[3 * (i * dim + j)] = buffer[3 * (k * width + h)];
            buffer2[3 * (i * dim + j) + 1] = buffer[3 * (k * width + h) + 1];
            buffer2[3 * (i * dim + j) + 2] = buffer[3 * (k * width + h) + 2];

        }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim, dim, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer2);

    free(buffer);
    free(buffer2);
}
