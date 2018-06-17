#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>

const GLint WIDTH = 800, HEIGHT = 600;
const float VERTICES[] = 
{
  // Position (XYZ)      Color (RGB)
   0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   // Top-Right
   0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   // Bottom-Right
  -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   // Bottom-Left
  -0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 0.0f    // Top-Left
};
const u_int VERTEX_INDICES[] =
{
  0, 1, 3, // First Triangle
  1, 2, 3  // Second Triangle
};

class Game
{
public:
  Game(SDL_Window *window, SDL_GLContext context);
  ~Game();
  void Run();
  bool IsReady() { return _init; }

private:
  void createVAO();
  bool handleEvent(SDL_Event e);
  void update();
  void render();
  bool loadShader(std::string filename, u_int *id, int shaderFlag);

  SDL_Window *_window;
  SDL_GLContext _context;
  u_int _vertextBufferObject;
  u_int _vertexArrayObject;
  u_int _elementBufferObject;
  u_int _shaderProgram;
  bool _init = true;
};

#endif