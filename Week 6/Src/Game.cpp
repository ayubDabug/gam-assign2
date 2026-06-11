#include "Game.h"

#include <algorithm>

Game::Game(
    GLFWwindow* window,
    int width,
    int height)
{
    this->window =
        window;

    screenWidth =
        width;

    screenHeight =
        height;
}

//--------------------------------------------------
// Initialize
//--------------------------------------------------

bool Game::Initialize(
    Shader* shader)
{
    if (!renderer.Initialize())
    {
        return false;
    }

    renderer.SetShader(
        shader);

    map.Generate();

    SpawnPlayer();

    SpawnEnemies();

    return true;
}

//--------------------------------------------------
// Spawn Player
//--------------------------------------------------

void Game::SpawnPlayer()
{
    player.SetPosition(
        map.GetPlayerSpawn());

    player.SetRotation(
        0.0f);

    player.SetMoveSpeed(
        220.0f);

    player.SetRotationSpeed(
        3.0f);

    player.SetSize(
        32.0f,
        32.0f);
}

//--------------------------------------------------
// Spawn Enemies
//--------------------------------------------------

void Game::SpawnEnemies()
{
    enemies.clear();

    const int ENEMY_COUNT = 4;

    for (int i = 0;
        i < ENEMY_COUNT;
        i++)
    {
        Enemy enemy;

        enemy.SetPosition(
            map.GetEnemySpawn(
                i));

        enemy.SetMoveSpeed(
            120.0f);

        enemy.SetSize(
            32.0f,
            32.0f);

        enemies.push_back(
            enemy);
    }
}

//--------------------------------------------------
// Input
//--------------------------------------------------

void Game::ProcessInput(
    float deltaTime)
{
    HandlePlayerInput(
        deltaTime);
}

//--------------------------------------------------
// Player Input
//--------------------------------------------------

void Game::HandlePlayerInput(
    float deltaTime)
{
    if (!player.IsAlive())
    {
        return;
    }

    if (
        glfwGetKey(
            window,
            GLFW_KEY_A) ==
        GLFW_PRESS ||

        glfwGetKey(
            window,
            GLFW_KEY_LEFT) ==
        GLFW_PRESS)
    {
        player.RotateLeft(
            deltaTime);
    }

    if (
        glfwGetKey(
            window,
            GLFW_KEY_D) ==
        GLFW_PRESS ||

        glfwGetKey(
            window,
            GLFW_KEY_RIGHT) ==
        GLFW_PRESS)
    {
        player.RotateRight(
            deltaTime);
    }

    glm::vec2 oldPosition =
        player.GetPosition();

    if (
        glfwGetKey(
            window,
            GLFW_KEY_W) ==
        GLFW_PRESS ||

        glfwGetKey(
            window,
            GLFW_KEY_UP) ==
        GLFW_PRESS)
    {
        player.MoveForward(
            deltaTime);

        if (
            map.CheckTankCollision(
                player.GetPosition(),
                player.GetWidth(),
                player.GetHeight()))
        {
            player.SetPosition(
                oldPosition);
        }
    }

    oldPosition =
        player.GetPosition();

    if (
        glfwGetKey(
            window,
            GLFW_KEY_S) ==
        GLFW_PRESS ||

        glfwGetKey(
            window,
            GLFW_KEY_DOWN) ==
        GLFW_PRESS)
    {
        player.MoveBackward(
            deltaTime);

        if (
            map.CheckTankCollision(
                player.GetPosition(),
                player.GetWidth(),
                player.GetHeight()))
        {
            player.SetPosition(
                oldPosition);
        }
    }

    static bool previousSpace =
        false;

    bool currentSpace =
        glfwGetKey(
            window,
            GLFW_KEY_SPACE) ==
        GLFW_PRESS;

    if (
        currentSpace &&
        !previousSpace)
    {
        Bullet bullet;

        bullet.SetPosition(
            player.GetPosition());

        bullet.SetDirection(
            player.GetForwardVector());

        bullet.SetSpeed(
            500.0f);

        bullet.SetFromPlayer(
            true);

        bullets.push_back(
            bullet);
    }

    previousSpace =
        currentSpace;
}

//--------------------------------------------------
// Update
//--------------------------------------------------

void Game::Update(
    float deltaTime)
{
    UpdatePlayer(
        deltaTime);

    UpdateEnemies(
        deltaTime);

    UpdateBullets(
        deltaTime);

    CheckCollisions();

    RemoveDestroyedObjects();
}

//--------------------------------------------------
// Player
//--------------------------------------------------

void Game::UpdatePlayer(
    float deltaTime)
{
    player.Update(
        deltaTime);

    glm::vec2 position =
        player.GetPosition();

    position.x =
        std::clamp(
            position.x,
            16.0f,
            static_cast<float>(
                screenWidth - 16));

    position.y =
        std::clamp(
            position.y,
            16.0f,
            static_cast<float>(
                screenHeight - 16));

    player.SetPosition(
        position);
}

//--------------------------------------------------
// Enemies
//--------------------------------------------------

void Game::UpdateEnemies(
    float deltaTime)
{
    for (Enemy& enemy : enemies)
    {
        if (!enemy.IsAlive())
        {
            continue;
        }

        enemy.SetTargetPosition(
            player.GetPosition());

        glm::vec2 oldPosition =
            enemy.GetPosition();

        enemy.Update(
            deltaTime);

        if (
            map.CheckTankCollision(
                enemy.GetPosition(),
                enemy.GetWidth(),
                enemy.GetHeight()))
        {
            enemy.SetPosition(
                oldPosition);
        }

        glm::vec2 position =
            enemy.GetPosition();

        position.x =
            std::clamp(
                position.x,
                16.0f,
                static_cast<float>(
                    screenWidth - 16));

        position.y =
            std::clamp(
                position.y,
                16.0f,
                static_cast<float>(
                    screenHeight - 16));

        enemy.SetPosition(
            position);

        if (
            enemy.CanSeeTarget() &&
            enemy.DistanceToTarget()
            <= enemy.GetShootingRange())
        {
            if (
                enemy.CanShoot())
            {
                Bullet bullet;

                bullet.SetPosition(
                    enemy.GetPosition());

                bullet.SetDirection(
                    enemy.DirectionToTarget());

                bullet.SetSpeed(
                    400.0f);

                bullet.SetFromPlayer(
                    false);

                bullets.push_back(
                    bullet);

                enemy.ResetFireTimer();
            }
        }
    }
}

//--------------------------------------------------
// Bullets
//--------------------------------------------------

void Game::UpdateBullets(
    float deltaTime)
{
    for (Bullet& bullet : bullets)
    {
        if (!bullet.IsActive())
        {
            continue;
        }

        bullet.Update(
            deltaTime);

        if (
            bullet.IsOutsideScreen(
                screenWidth,
                screenHeight))
        {
            bullet.Deactivate();
            continue;
        }

        if (
            map.IsBlocked(
                bullet.GetPosition()))
        {
            map.DamageTile(
                bullet.GetPosition());

            bullet.Deactivate();
        }
    }
}
//--------------------------------------------------
// Collision
//--------------------------------------------------

void Game::CheckCollisions()
{
    for (Bullet& bullet : bullets)
    {
        if (!bullet.IsActive())
        {
            continue;
        }

        //--------------------------------------------------
        // Enemy Hit
        //--------------------------------------------------

        if (bullet.IsFromPlayer())
        {
            for (Enemy& enemy : enemies)
            {
                if (!enemy.IsAlive())
                {
                    continue;
                }

                glm::vec2 bulletMin =
                    bullet.GetMinBounds();

                glm::vec2 bulletMax =
                    bullet.GetMaxBounds();

                glm::vec2 enemyMin =
                    enemy.GetMinBounds();

                glm::vec2 enemyMax =
                    enemy.GetMaxBounds();

                bool overlap =
                    bulletMin.x < enemyMax.x &&
                    bulletMax.x > enemyMin.x &&
                    bulletMin.y < enemyMax.y &&
                    bulletMax.y > enemyMin.y;

                if (overlap)
                {
                    enemy.Destroy();

                    bullet.Deactivate();

                    break;
                }
            }
        }

        //--------------------------------------------------
        // Player Hit
        //--------------------------------------------------

        else
        {
            glm::vec2 bulletMin =
                bullet.GetMinBounds();

            glm::vec2 bulletMax =
                bullet.GetMaxBounds();

            glm::vec2 playerMin =
                player.GetMinBounds();

            glm::vec2 playerMax =
                player.GetMaxBounds();

            bool overlap =
                bulletMin.x < playerMax.x &&
                bulletMax.x > playerMin.x &&
                bulletMin.y < playerMax.y &&
                bulletMax.y > playerMin.y;

            if (overlap)
            {
                player.Destroy();

                bullet.Deactivate();
            }
        }
    }
}

//--------------------------------------------------
// Cleanup
//--------------------------------------------------

void Game::RemoveDestroyedObjects()
{
    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),

            [](const Bullet& bullet)
            {
                return
                    !bullet.IsActive();
            }),

        bullets.end());

    enemies.erase(
        std::remove_if(
            enemies.begin(),
            enemies.end(),

            [](const Enemy& enemy)
            {
                return
                    !enemy.IsAlive();
            }),

        enemies.end());
}

//--------------------------------------------------
// Render
//--------------------------------------------------

void Game::Render()
{
    renderer.BeginFrame();

    renderer.RenderMap(
        map);

    renderer.RenderPlayer(
        player);

    renderer.RenderEnemies(
        enemies);

    renderer.RenderBullets(
        bullets);

    renderer.EndFrame();
}

//--------------------------------------------------
// Shutdown
//--------------------------------------------------

void Game::Shutdown()
{
    renderer.Shutdown();

    bullets.clear();

    enemies.clear();
}

//--------------------------------------------------
// Accessors
//--------------------------------------------------

Tank& Game::GetPlayer()
{
    return player;
}

std::vector<Enemy>&
Game::GetEnemies()
{
    return enemies;
}

std::vector<Bullet>&
Game::GetBullets()
{
    return bullets;
}

Map& Game::GetMap()
{
    return map;
}

Renderer& Game::GetRenderer()
{
    return renderer;
}