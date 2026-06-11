#include "Renderer.h"

#include <vector>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

namespace
{
    void AddTriangle(
        std::vector<float>& vertices,

        float x1, float y1,
        float x2, float y2,
        float x3, float y3,

        float r,
        float g,
        float b)
    {
        vertices.insert(
            vertices.end(),
            {
                x1,y1,0.0f,r,g,b,
                x2,y2,0.0f,r,g,b,
                x3,y3,0.0f,r,g,b
            });
    }

    std::vector<float> CreateRectangle(
        float width,
        float height,

        float r,
        float g,
        float b)
    {
        std::vector<float> vertices;

        float hw =
            width * 0.5f;

        float hh =
            height * 0.5f;

        AddTriangle(
            vertices,

            -hw, -hh,
            hw, -hh,
            hw, hh,

            r, g, b);

        AddTriangle(
            vertices,

            -hw, -hh,
            hw, hh,
            -hw, hh,

            r, g, b);

        return vertices;
    }
}

Renderer::Renderer()
{
    shader = nullptr;

    mvpLocation = -1;

    playerBodyMesh = nullptr;
    playerTurretMesh = nullptr;

    enemyBodyMesh = nullptr;
    enemyTurretMesh = nullptr;

    bulletMesh = nullptr;

    brickMesh = nullptr;
    steelMesh = nullptr;
}

bool Renderer::Initialize()
{
    projection =
        glm::ortho(
            0.0f,
            1280.0f,
            0.0f,
            720.0f,
            -1.0f,
            1.0f);

    view =
        glm::mat4(1.0f);

    //--------------------------------------------------
    // Player
    //--------------------------------------------------

    playerBodyMesh =
        new Mesh(
            CreateRectangle(
                32.0f,
                32.0f,

                0.0f,
                0.8f,
                0.0f));

    playerTurretMesh =
        new Mesh(
            CreateRectangle(
                24.0f,
                6.0f,

                0.2f,
                1.0f,
                0.2f));

    //--------------------------------------------------
    // Enemy
    //--------------------------------------------------

    enemyBodyMesh =
        new Mesh(
            CreateRectangle(
                32.0f,
                32.0f,

                0.8f,
                0.0f,
                0.0f));

    enemyTurretMesh =
        new Mesh(
            CreateRectangle(
                24.0f,
                6.0f,

                1.0f,
                0.2f,
                0.2f));

    //--------------------------------------------------
    // Bullet
    //--------------------------------------------------

    bulletMesh =
        new Mesh(
            CreateRectangle(
                8.0f,
                8.0f,

                1.0f,
                1.0f,
                0.0f));

    //--------------------------------------------------
    // Brick
    //--------------------------------------------------

    brickMesh =
        new Mesh(
            CreateRectangle(
                32.0f,
                32.0f,

                0.55f,
                0.27f,
                0.07f));

    //--------------------------------------------------
    // Steel
    //--------------------------------------------------

    steelMesh =
        new Mesh(
            CreateRectangle(
                32.0f,
                32.0f,

                0.65f,
                0.65f,
                0.65f));

    return true;
}

void Renderer::SetShader(
    Shader* shaderProgram)
{
    shader =
        shaderProgram;

    if (shader)
    {
        mvpLocation =
            static_cast<int>(
                glGetUniformLocation(
                    shader->GetID(),
                    "mvp"));
    }
}

void Renderer::BeginFrame()
{
    glClearColor(
        0.0f,
        0.0f,
        0.0f,
        1.0f);

    glClear(
        GL_COLOR_BUFFER_BIT);

    if (shader)
    {
        shader->Use();
    }
}

void Renderer::EndFrame()
{
}

void Renderer::DrawMesh(
    Mesh* mesh,
    const glm::mat4& model)
{
    if (!mesh)
    {
        return;
    }

    glm::mat4 mvp =
        projection *
        view *
        model;

    glUniformMatrix4fv(
        mvpLocation,
        1,
        GL_FALSE,
        glm::value_ptr(
            mvp));

    mesh->Draw();
}

void Renderer::DrawTank(
    const Tank& tank,
    Mesh* bodyMesh,
    Mesh* turretMesh)
{
    glm::vec2 position =
        tank.GetPosition();

    float rotation =
        tank.GetRotation();

    glm::mat4 bodyModel =
        glm::translate(
            glm::mat4(1.0f),
            glm::vec3(
                position.x,
                position.y,
                0.0f));

    bodyModel =
        glm::rotate(
            bodyModel,
            rotation,
            glm::vec3(
                0.0f,
                0.0f,
                1.0f));

    DrawMesh(
        bodyMesh,
        bodyModel);

    glm::mat4 turretModel =
        glm::translate(
            glm::mat4(1.0f),
            glm::vec3(
                position.x,
                position.y,
                0.0f));

    turretModel =
        glm::rotate(
            turretModel,
            rotation,
            glm::vec3(
                0.0f,
                0.0f,
                1.0f));

    turretModel =
        glm::translate(
            turretModel,
            glm::vec3(
                12.0f,
                0.0f,
                0.0f));

    DrawMesh(
        turretMesh,
        turretModel);
}

void Renderer::DrawBullet(
    const Bullet& bullet)
{
    glm::vec2 position =
        bullet.GetPosition();

    glm::mat4 model =
        glm::translate(
            glm::mat4(1.0f),
            glm::vec3(
                position.x,
                position.y,
                0.0f));

    DrawMesh(
        bulletMesh,
        model);
}

void Renderer::DrawTile(
    const Tile& tile)
{
    Mesh* mesh =
        nullptr;

    if (tile.type ==
        TileType::Breakable)
    {
        mesh =
            brickMesh;
    }
    else if (
        tile.type ==
        TileType::Steel)
    {
        mesh =
            steelMesh;
    }

    if (!mesh)
    {
        return;
    }

    glm::mat4 model =
        glm::translate(
            glm::mat4(1.0f),
            glm::vec3(
                tile.position.x + 16.0f,
                tile.position.y + 16.0f,
                0.0f));

    DrawMesh(
        mesh,
        model);
}

void Renderer::RenderPlayer(
    const Tank& player)
{
    if (!player.IsAlive())
    {
        return;
    }

    DrawTank(
        player,
        playerBodyMesh,
        playerTurretMesh);
}

void Renderer::RenderEnemies(
    const std::vector<Enemy>& enemies)
{
    for (const Enemy& enemy : enemies)
    {
        if (!enemy.IsAlive())
        {
            continue;
        }

        DrawTank(
            enemy,
            enemyBodyMesh,
            enemyTurretMesh);
    }
}

void Renderer::RenderBullets(
    const std::vector<Bullet>& bullets)
{
    for (const Bullet& bullet : bullets)
    {
        if (!bullet.IsActive())
        {
            continue;
        }

        DrawBullet(
            bullet);
    }
}

void Renderer::RenderMap(
    const Map& map)
{
    const std::vector<Tile>& tiles =
        map.GetTiles();

    for (const Tile& tile : tiles)
    {
        DrawTile(
            tile);
    }
}

void Renderer::Shutdown()
{
    delete playerBodyMesh;
    playerBodyMesh = nullptr;

    delete playerTurretMesh;
    playerTurretMesh = nullptr;

    delete enemyBodyMesh;
    enemyBodyMesh = nullptr;

    delete enemyTurretMesh;
    enemyTurretMesh = nullptr;

    delete bulletMesh;
    bulletMesh = nullptr;

    delete brickMesh;
    brickMesh = nullptr;

    delete steelMesh;
    steelMesh = nullptr;
}

Renderer::~Renderer()
{
    Shutdown();
}