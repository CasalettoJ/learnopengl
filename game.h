#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <OpenGL/glu.h>

const GLint WIDTH = 800, HEIGHT = 600;

class Game
{
  public:
    Game(SDL_Window *window, SDL_GLContext context);
    ~Game();
    void Run();
    bool IsReady() { return _init; }

  private:
    bool handleEvent(SDL_Event e);
    void update();
    void render();
    SDL_Window *_window;
    SDL_GLContext _context;
    bool _init;
};

#endif