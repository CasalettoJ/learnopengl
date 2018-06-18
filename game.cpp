#include <iostream>
#include <fstream>
#include <sstream>

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#include "game.h"
#include "systems/shader/shader.h"
#include "systems/media/texture.h"

Game::Game(SDL_Window *window, SDL_GLContext context)
{
    _window = window;
    _context = context;

    // Create vertex buffer object that will bind to array buffer for vertices
    glGenBuffers(1, &_vertextBufferObject);

    // Create the VAO that will render the triangle vertices
    glGenVertexArrays(1, &_vertexArrayObject);

    // Create the EBO that will render the rectangle from two sets of vertex indices
    glGenBuffers(1, &_elementBufferObject);

    // Get the test texture
    _textureId = CreateTextureFromFile("assets/textures/drake.png");
    if (_textureId == 0)
    {
        _init = false;
    }

    // Compile and link the shaders into a test program
    unsigned int vertShaderId = LoadShader("assets/shaders/basic.vert", GL_VERTEX_SHADER);
    unsigned int fragShaderId = LoadShader("assets/shaders/basic.frag", GL_FRAGMENT_SHADER);
    if (vertShaderId == 0 || fragShaderId == 0)
    {
        _init = false;
    }

    _shaderProgram = CreateProgram(vertShaderId, fragShaderId);
    if (_shaderProgram == 0)
    {
        _init = false;
    }

    // Set up the VAO that will render the rectangle
    createVAO();

    // Set up the texture for the shader program's texture sampler.
    glUseProgram(_shaderProgram);
    glUniform1i(glGetUniformLocation(_textureId, "ourTexture"), 0);

    // delete compiled shaders since they aren't needed after program linking
    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);

    // Enable blend mode for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    glDeleteVertexArrays(1, &_vertexArrayObject);
    glDeleteBuffers(1, &_vertextBufferObject);
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

    // Use the shaderProgram created on initialization for vertex/fragment shaders
    glUseProgram(_shaderProgram);
    // Use our test texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    // Use the VAO that buffers vertices to array buffer
    glBindVertexArray(_vertexArrayObject);
    // Draw the triangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Unbind the VAO
    glBindVertexArray(0);

    // Draw OpenGL
    SDL_GL_SwapWindow(_window);
}

void Game::createVAO()
{
    glBindVertexArray(_vertexArrayObject);
    // Bind the vertices to the ARRAY_BUFFER before buffering vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vertextBufferObject);
    // Buffer vertices into the _VBO using the bound ARRAY_BUFFER for the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    // Bind and buffer the index array for opengl to determine which vertices to use in what order.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VERTEX_INDICES), VERTEX_INDICES, GL_STATIC_DRAW);
    // Copy 
    /*
     * Set up the single vertex attribute (aPos) in our vertex shader:
     * BASIC.VERT
     * 
     * in vec2 aPos
     *      Represents a point in 2d space.
     * location: 0
     * 
     * in vec3 aColor
     *      Represents a color for the vertex.
     * location: 1
     * 
     * in vec2 aTexCoord
     *      Coordinate mapping to supplied texture for vertex.
     * location: 2
     * 
     *   __________     aPos (0)   aColor (1)   aTexCoord (2)
     *   | VERTEX |     X | Y      R | G | B    S | T
     *   __________     8 bytes    12 bytes     8 bytes
     *             
     *   Stride: 28 bytes ----------------------------------->
     * 
    */
   
    //aPos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //aColor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    // //aTexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5*sizeof(float)));
    glEnableVertexAttribArray(2);
    // Unbding VAO
    glBindVertexArray(0);
}