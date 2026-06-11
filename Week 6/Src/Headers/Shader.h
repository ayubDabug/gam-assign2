#ifndef SHADER_H
#define SHADER_H

class Shader
{
private:

    unsigned int programID;

public:

    Shader(
        const char* vertexPath,
        const char* fragmentPath);

    void Use();

    unsigned int GetID() const;
};

#endif