#include <glew.h>
#include <glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Game.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

int main()
{
    //--------------------------------------------------
    // GLFW
    //--------------------------------------------------

    if (!glfwInit())
    {
        std::cout
            << "Failed to initialize GLFW"
            << std::endl;

        return -1;
    }

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3);

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE);

    //--------------------------------------------------
    // Window
    //--------------------------------------------------

    GLFWwindow* window =
        glfwCreateWindow(
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            "Tank Battle",
            nullptr,
            nullptr);

    if (!window)
    {
        std::cout
            << "Failed to create window"
            << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(
        window);

    //--------------------------------------------------
    // GLEW
    //--------------------------------------------------

    glewExperimental =
        GL_TRUE;

    GLenum glewResult =
        glewInit();

    if (glewResult != GLEW_OK)
    {
        std::cout
            << "GLEW ERROR: "
            << glewGetErrorString(
                glewResult)
            << std::endl;

        glfwDestroyWindow(
            window);

        glfwTerminate();

        return -1;
    }

    //--------------------------------------------------
    // OpenGL
    //--------------------------------------------------

    glViewport(
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT);

    glEnable(
        GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA);

    //--------------------------------------------------
    // Shader
    //--------------------------------------------------

    Shader shader(
        "Src/Shaders/vertex.glsl",
        "Src/Shaders/fragment.glsl");

    //--------------------------------------------------
    // Game
    //--------------------------------------------------

    Game game(
        window,
        WINDOW_WIDTH,
        WINDOW_HEIGHT);

    if (!game.Initialize(
        &shader))
    {
        std::cout
            << "Game initialization failed"
            << std::endl;

        glfwDestroyWindow(
            window);

        glfwTerminate();

        return -1;
    }

    //--------------------------------------------------
    // Timing
    //--------------------------------------------------

    float lastTime =
        static_cast<float>(
            glfwGetTime());

    //--------------------------------------------------
    // Main Loop
    //--------------------------------------------------

    while (
        !glfwWindowShouldClose(
            window))
    {
        float currentTime =
            static_cast<float>(
                glfwGetTime());

        float deltaTime =
            currentTime -
            lastTime;

        lastTime =
            currentTime;

        //--------------------------------------------------
        // ESC
        //--------------------------------------------------

        if (
            glfwGetKey(
                window,
                GLFW_KEY_ESCAPE)
            ==
            GLFW_PRESS)
        {
            glfwSetWindowShouldClose(
                window,
                GLFW_TRUE);
        }

        //--------------------------------------------------
        // Update
        //--------------------------------------------------

        game.ProcessInput(
            deltaTime);

        game.Update(
            deltaTime);

        //--------------------------------------------------
        // Render
        //--------------------------------------------------

        game.Render();

        //--------------------------------------------------
        // Present
        //--------------------------------------------------

        glfwSwapBuffers(
            window);

        glfwPollEvents();
    }

    //--------------------------------------------------
    // Shutdown
    //--------------------------------------------------

    game.Shutdown();

    glfwDestroyWindow(
        window);

    glfwTerminate();

    return 0;
}