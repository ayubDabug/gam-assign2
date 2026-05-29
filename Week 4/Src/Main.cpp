#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <filesystem>
#include <iostream>

#include "Shader.h"
#include "Mesh.h"
#include "Renderer.h"

/*
    Main Function

    Entry point of the application.

    Responsibilities:

    - Initialize GLFW
    - Create the OpenGL window
    - Initialize GLEW
    - Configure OpenGL settings
    - Create project objects
    - Execute the render loop
    - Release resources before exiting
*/
int main()
{
    /*
        Initialize GLFW.

        GLFW is responsible for window creation,
        OpenGL context management, and user input.
    */
    if (!glfwInit())
    {
        std::cout
            << "Failed to initialize GLFW."
            << std::endl;

        return -1;
    }

    /*
        Configure OpenGL version.

        This project uses OpenGL 3.3 Core Profile.
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
        Create the application window.

        Parameters:

        Width  = 800
        Height = 600
        Title  = "3D RGB Cube"
    */
    GLFWwindow* window =
        glfwCreateWindow(
            800,
            600,
            "3D RGB Cube",
            nullptr,
            nullptr);

    /*
        Verify that the window was created
        successfully.
    */
    if (!window)
    {
        std::cout
            << "Failed to create window."
            << std::endl;

        glfwTerminate();
        return -1;
    }

    /*
        Make the OpenGL context associated
        with this window the active context.
    */
    glfwMakeContextCurrent(window);

    /*
        Display the current working directory.

        Useful for debugging relative file paths,
        such as shader locations.
    */
    std::cout
        << "Current Path: "
        << std::filesystem::current_path()
        << std::endl;

    /*
        Initialize GLEW.

        GLEW loads OpenGL extensions and
        modern OpenGL functionality.
    */
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout
            << "Failed to initialize GLEW."
            << std::endl;

        glfwTerminate();
        return -1;
    }

    /*
        Configure the viewport.

        The viewport defines the area of
        the window used for rendering.
    */
    glViewport(0, 0, 800, 600);

    /*
        Enable depth testing.

        Depth testing ensures that objects
        closer to the camera appear in front
        of objects farther away.

        This is required for proper 3D rendering.
    */
    glEnable(GL_DEPTH_TEST);

    /*
        Create the shader object.

        Loads and compiles the vertex and
        fragment shaders used for rendering.
    */
    Shader shader(
        "Src/Shaders/vertex.glsl",
        "Src/Shaders/fragment.glsl");

    /*
        Create the cube mesh.

        The mesh contains vertex data,
        color data, and EBO indices.
    */
    Mesh cube;

    /*
        Create the renderer.

        The renderer connects the shader
        and mesh objects and performs
        the rendering operations.
    */
    Renderer renderer(
        &shader,
        &cube);

    /*
        Main Application Loop.

        Continues running until the user
        closes the window.
    */
    while (!glfwWindowShouldClose(window))
    {
        
        /*
           Clear both the color buffer
           and depth buffer before
           rendering the next frame.
       */
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
            Set the background color.

            RGBA:
            R = 0.1
            G = 0.1
            B = 0.15
            A = 1.0
        */

        glClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f);

        /*
            Close the application when
            the Escape key is pressed.
        */
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(
                window,
                true);
        }


        /*
            Calculate the rotation angle.

            glfwGetTime() returns the elapsed
            time since GLFW was initialized.

            The angle is used to rotate the
            cube continuously around the Y axis.
        */
        float angle =
            static_cast<float>(
                glfwGetTime());

        /*
            Render the cube.
        */
        renderer.Render(angle);

        /*
            Display the rendered frame
            and process window events.
        */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
        Release GLFW resources before exiting.
    */
    glfwTerminate();

    return 0;
}