#ifndef SYSTEM_TEXTURE_H
#define SYSTEM_TEXTURE_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

///
/// Loads a texture file and returns the uint of a generated texture ID.
/// Returns 0 if there was an error.
///
unsigned int CreateTextureFromFile(std::string filename)
{
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
    {
        std::cout << "Unable to load image file: " << filename << std::endl << IMG_GetError() << std::endl;
        return 0;
    }

    unsigned int textureId = 0;
    glGenTextures(1, &textureId);
    if (textureId == 0)
    {
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}

#endif