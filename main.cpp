

#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "game.h"

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "Failure to initialize SDL" << std::endl
                  << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 - OpenGL4.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (window == nullptr)
    {
        std::cout << "Failure to create window" << std::endl
                  << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    glewExperimental = true;
    GLenum initResult = glewInit();
    if (GLEW_OK != initResult)
    {
        std::cout << "Failed to initialize GLEW" << std::endl
                  << glewGetErrorString(initResult) << std::endl;
        return false;
    }
    glViewport(0, 0, WIDTH, HEIGHT);

    std::unique_ptr<Game> game(new Game(window, context));
    if (!game->IsReady())
    {
        std::cout << "Exiting with errors." << std::endl;
        return EXIT_FAILURE;
    }
    game->Run();

    SDL_Quit();
    return EXIT_SUCCESS;
}