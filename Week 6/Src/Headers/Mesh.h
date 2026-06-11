#ifndef MESH_H
#define MESH_H

#include <vector>

class Mesh
{
private:

    unsigned int VAO;

    unsigned int VBO;

    unsigned int vertexCount;

public:

    Mesh(
        const std::vector<float>& vertices);

    void Draw() const;

    ~Mesh();
};

#endif