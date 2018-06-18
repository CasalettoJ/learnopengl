#ifndef SYSTEM_SHADER_H
#define SYSTEM_SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <array>

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

///
/// Loads and compiles glsl source file, returning its Id.
/// Returns 0 if there was an error.
///
unsigned int LoadShader(std::string filename, int shaderFlag)
{
    // Open the file
    std::ifstream sourceFile{filename, std::ios::in | std::ios::binary};
    if (!sourceFile)
    {
        std::cout << "Failed to open shader source file." << std::endl;
        return 0;
    }

    // Read the file buffer into a stringstream, save as c_str to compile.
    std::string source{std::istreambuf_iterator<char>{sourceFile},{}};
    const char *sourceString = source.c_str();
    sourceFile.close();

    // Create shader, set source of the shader, and compile.
    std::cout << "Compiling source " << filename << "===============================" << std::endl << source << std::endl << std::endl;
    unsigned int shaderId = glCreateShader(shaderFlag);
    glShaderSource(shaderId, 1, &sourceString, nullptr);
    glCompileShader(shaderId);

    // Check for compilation errors
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::array<char, 1024> infoLog;
        glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog.data());
        std::cout << "Shader compile failure: " << std::endl << infoLog.data() << std::endl;
        return 0;
    }

    return shaderId;
}

///
/// Creates a program and returns its Id.
/// Parameters should be GLuint shader Ids.
/// Returns 0 if there was an error.
///
template<typename... Args>
unsigned int CreateProgram(Args... shaderIds)
{
    unsigned int shaderProgram = glCreateProgram();

    // Attach all of the supplied shader Ids
    const int shaderCount = sizeof...(shaderIds);
    std::array<unsigned int, shaderCount> shaders = {{shaderIds...}};
    for (uint8_t i = 0; i < shaderCount; ++i)
    {
        glAttachShader(shaderProgram, shaders[i]);
    }
    glLinkProgram(shaderProgram);

    // Error check the linking
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::array<char, 1024> infoLog;
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog.data());
        std::cout << "Failure linking shader program:" << std::endl << infoLog.data() << std::endl;
        return 0;
    }

    return shaderProgram;
}

#endif