#ifndef RENDERER_H
#define RENDERER_H

#include <glew.h>

#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Mesh.h"

#include "Tank.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Map.h"

class Renderer
{
private:

    //--------------------------------------------------
    // Shader
    //--------------------------------------------------

    Shader* shader;

    //--------------------------------------------------
    // Uniforms
    //--------------------------------------------------

    int mvpLocation;

    //--------------------------------------------------
    // Matrices
    //--------------------------------------------------

    glm::mat4 projection;

    glm::mat4 view;

    //--------------------------------------------------
    // Player Meshes
    //--------------------------------------------------

    Mesh* playerBodyMesh;

    Mesh* playerTurretMesh;

    //--------------------------------------------------
    // Enemy Meshes
    //--------------------------------------------------

    Mesh* enemyBodyMesh;

    Mesh* enemyTurretMesh;

    //--------------------------------------------------
    // Bullet Mesh
    //--------------------------------------------------

    Mesh* bulletMesh;

    //--------------------------------------------------
    // Map Meshes
    //--------------------------------------------------

    Mesh* brickMesh;

    Mesh* steelMesh;

private:

    //--------------------------------------------------
    // Internal Draw
    //--------------------------------------------------

    void DrawMesh(
        Mesh* mesh,
        const glm::mat4& model);

    void DrawTank(
        const Tank& tank,
        Mesh* bodyMesh,
        Mesh* turretMesh);

    void DrawBullet(
        const Bullet& bullet);

    void DrawTile(
        const Tile& tile);

public:

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Renderer();

    //--------------------------------------------------
    // Initialization
    //--------------------------------------------------

    bool Initialize();

    //--------------------------------------------------
    // Shader
    //--------------------------------------------------

    void SetShader(
        Shader* shaderProgram);

    //--------------------------------------------------
    // Frame
    //--------------------------------------------------

    void BeginFrame();

    void EndFrame();

    //--------------------------------------------------
    // Render Passes
    //--------------------------------------------------

    void RenderPlayer(
        const Tank& player);

    void RenderEnemies(
        const std::vector<Enemy>& enemies);

    void RenderBullets(
        const std::vector<Bullet>& bullets);

    void RenderMap(
        const Map& map);

    //--------------------------------------------------
    // Cleanup
    //--------------------------------------------------

    void Shutdown();

    //--------------------------------------------------
    // Destructor
    //--------------------------------------------------

    ~Renderer();
};

#endif