#include <SDL2/SDL.h>
#include <iostream>
#include "game.h"

Game::Game(SDL_Window *window, SDL_GLContext context)
{
    _window = window;
    _context = context;
}

Game::~Game()
{
    SDL_GL_DeleteContext(_context);
    SDL_DestroyWindow(_window);
}

void Game::Run()
{

    std::cout << "Running Game" << std::endl;
    bool _quit = false;
    SDL_Event e;
    while (true)
    {
        while (SDL_PollEvent(&e))
        {
            bool shouldQuit = !handleEvent(e);
            _quit = _quit ? _quit : shouldQuit;
        }
        if (_quit)
        {
            break;
        }
        update();
        render();
    }
}

bool Game::handleEvent(SDL_Event e)
{
    switch (e.type)
    {
    case SDL_QUIT:
        return false;
    }
    return true;
}

void Game::update()
{
    // Update Logic Here
}

void Game::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw OpenGL
    SDL_GL_SwapWindow(_window);
}
