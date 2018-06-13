#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include "game.h"

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
    createVAO();

    // Load and compile shaders
    u_int _vertShaderId;
    u_int _fragShaderId;
    bool loadVert = loadShader("assets/shaders/basic.vert", &_vertShaderId, GL_VERTEX_SHADER);
    bool loadFrag = loadShader("assets/shaders/basic.frag", &_fragShaderId, GL_FRAGMENT_SHADER);
    if (!loadVert || !loadFrag)
    {
        std::cout << "Shader load failure." << std::endl;
        _init = false;
    }

    // Create the shader program with the compiled shaders
    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, _vertShaderId);
    glAttachShader(_shaderProgram, _fragShaderId);
    glLinkProgram(_shaderProgram);
    // Error check the shader linking
    int shaderSuccess;
    char infoLog[512];
    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &shaderSuccess);
    if (!shaderSuccess)
    {
        glGetProgramInfoLog(_shaderProgram, 512, nullptr, infoLog);
        std::cout << "Failure linking shader program:" << std::endl << infoLog << std::endl;
        _init = false;
    }

    // delete compiled shaders since they aren't needed after program linking
    glDeleteShader(_vertShaderId);
    glDeleteShader(_fragShaderId);
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
    // Use the shaderProgram created on initialization for vertex/fragment shaders
    glUseProgram(_shaderProgram);
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
     * (basic.vert)
     * 
     * in vec3 aPos
     *      Represents a point in 3d space, normalized.
     * location: 0
     * 
     *   ___________________________
     *   |          VERTEX         |
     *   ___________________________
     *   |    32bits(4 bytes) x    |
     *   |    32bits(4 bytes) y    |
     *   |    32bits(4 bytes) z    | 
     *   ___________________________
     *   Stride: 12bytes
     *   Offset: 0 (tightly packed)
     * 
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 );
    // Enable the attribute at the location layout (0)
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

bool Game::loadShader(std::string filename, u_int *id, int shaderFlag)
{
    // Load Shader
    std::ifstream shaderFile;
    std::stringstream buffer;
    shaderFile.open(filename);
    if (!shaderFile)
    {
        std::cout << "Failed to open shader file." << std::endl;
        return false;
    }
    buffer << shaderFile.rdbuf();

    const char *shader = buffer.str().c_str();
    buffer.clear();
    shaderFile.close();

    // Compile
    *id = glCreateShader(shaderFlag);
    glShaderSource(*id, 1, &shader, nullptr);
    glCompileShader(*id);

    // Error Check
    int success;
    char infoLog[512];
    glGetShaderiv(*id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(*id, 512, nullptr, infoLog);
        std::cout << "Failed to compile shader with error:" << std::endl << infoLog << std::endl;
        return false;
    }

    return true;
}