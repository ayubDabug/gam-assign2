#ifndef GAME_H
#define GAME_H

#include <vector>

#include <glew.h>
#include <glfw3.h>

#include "Tank.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Map.h"
#include "Renderer.h"

class Game
{
private:

    //--------------------------------------------------
    // Window
    //--------------------------------------------------

    GLFWwindow* window;

    int screenWidth;

    int screenHeight;

    //--------------------------------------------------
    // Systems
    //--------------------------------------------------

    Renderer renderer;

    Map map;

    //--------------------------------------------------
    // Entities
    //--------------------------------------------------

    Tank player;

    std::vector<Enemy> enemies;

    std::vector<Bullet> bullets;

    //--------------------------------------------------
    // Internal
    //--------------------------------------------------

    void SpawnPlayer();

    void SpawnEnemies();

    void HandlePlayerInput(
        float deltaTime);

    void UpdatePlayer(
        float deltaTime);

    void UpdateEnemies(
        float deltaTime);

    void UpdateBullets(
        float deltaTime);

    void CheckCollisions();

    void RemoveDestroyedObjects();

public:

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Game(
        GLFWwindow* window,
        int width,
        int height);

    //--------------------------------------------------
    // Initialization
    //--------------------------------------------------

    bool Initialize(
        Shader* shader);

    //--------------------------------------------------
    // Main Loop
    //--------------------------------------------------

    void ProcessInput(
        float deltaTime);

    void Update(
        float deltaTime);

    void Render();

    //--------------------------------------------------
    // Shutdown
    //--------------------------------------------------

    void Shutdown();

    //--------------------------------------------------
    // Accessors
    //--------------------------------------------------

    Tank& GetPlayer();

    std::vector<Enemy>&
        GetEnemies();

    std::vector<Bullet>&
        GetBullets();

    Map& GetMap();

    Renderer& GetRenderer();
};

#endif