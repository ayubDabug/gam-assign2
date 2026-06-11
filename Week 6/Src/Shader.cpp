#include "Shader.h"

#include <glew.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

Shader::Shader(
    const char* vertexPath,
    const char* fragmentPath)
{
    std::ifstream vertexFile(
        vertexPath);

    std::ifstream fragmentFile(
        fragmentPath);

    if (!vertexFile.is_open())
    {
        std::cout
            << "ERROR OPENING "
            << vertexPath
            << std::endl;
    }

    if (!fragmentFile.is_open())
    {
        std::cout
            << "ERROR OPENING "
            << fragmentPath
            << std::endl;
    }

    std::stringstream vertexStream;
    std::stringstream fragmentStream;

    vertexStream
        << vertexFile.rdbuf();

    fragmentStream
        << fragmentFile.rdbuf();

    std::string vertexCode =
        vertexStream.str();

    std::string fragmentCode =
        fragmentStream.str();

    const char* vertexSource =
        vertexCode.c_str();

    const char* fragmentSource =
        fragmentCode.c_str();

    GLint success;

    GLchar infoLog[1024];

    //--------------------------------------------------
    // Vertex Shader
    //--------------------------------------------------

    GLuint vertexShader =
        glCreateShader(
            GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexSource,
        nullptr);

    glCompileShader(
        vertexShader);

    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success);

    if (!success)
    {
        glGetShaderInfoLog(
            vertexShader,
            sizeof(infoLog),
            nullptr,
            infoLog);

        std::cout
            << infoLog
            << std::endl;
    }

    //--------------------------------------------------
    // Fragment Shader
    //--------------------------------------------------

    GLuint fragmentShader =
        glCreateShader(
            GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentSource,
        nullptr);

    glCompileShader(
        fragmentShader);

    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &success);

    if (!success)
    {
        glGetShaderInfoLog(
            fragmentShader,
            sizeof(infoLog),
            nullptr,
            infoLog);

        std::cout
            << infoLog
            << std::endl;
    }

    //--------------------------------------------------
    // Program
    //--------------------------------------------------

    programID =
        glCreateProgram();

    glAttachShader(
        programID,
        vertexShader);

    glAttachShader(
        programID,
        fragmentShader);

    glLinkProgram(
        programID);

    glGetProgramiv(
        programID,
        GL_LINK_STATUS,
        &success);

    if (!success)
    {
        glGetProgramInfoLog(
            programID,
            sizeof(infoLog),
            nullptr,
            infoLog);

        std::cout
            << infoLog
            << std::endl;
    }

    glDeleteShader(
        vertexShader);

    glDeleteShader(
        fragmentShader);
}

void Shader::Use()
{
    glUseProgram(
        programID);
}

unsigned int Shader::GetID() const
{
    return programID;
}