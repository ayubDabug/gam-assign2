// ==========================================================
// Include GLEW first
// GLEW loads modern OpenGL functions from the GPU driver
// ==========================================================
#include <glew.h>

// ==========================================================
// GLFW creates windows and OpenGL contexts
// Also handles keyboard/mouse input
// ==========================================================
#include <glfw3.h>

// ==========================================================
// Standard C++ input/output library
// Needed for cout
// ==========================================================
#include <iostream>

// Use standard namespace
using namespace std;

// ==========================================================
// Main function
// Program starts here
// ==========================================================
int main(void)
{
    // Pointer to a GLFW window object
    GLFWwindow* window;

    // ======================================================
    // Initialize GLFW library
    // ======================================================
    if (!glfwInit())
        return -1;

    // ======================================================
    // Tell GLFW which OpenGL version we want
    // OpenGL 3.3
    // ======================================================
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // ======================================================
    // Use OpenGL Core Profile
    // Removes old OpenGL functions like glBegin()
    // ======================================================
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ======================================================
    // Create window
    //
    // Parameters:
    // Width
    // Height
    // Window title
    // Monitor (NULL = windowed mode)
    // Shared context (NULL = none)
    // ======================================================
    window = glfwCreateWindow(
        640,
        480,
        "Modern OpenGL Triangle",
        NULL,
        NULL
    );

    // ======================================================
    // Check if window creation failed
    // ======================================================
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // ======================================================
    // Make this window's OpenGL context active
    // ======================================================
    glfwMakeContextCurrent(window);

    // ======================================================
    // Initialize GLEW
    //
    // GLEW loads all modern OpenGL functions
    // Must happen AFTER OpenGL context exists
    // ======================================================
    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }

    // ======================================================
    // Print OpenGL version
    // ======================================================
    cout << "OpenGL Version: "
        << glGetString(GL_VERSION)
        << endl;

    // ======================================================
    // Vertex Data
    //
    // Triangle coordinates
    //
    // X,Y pairs:
    //
    // (-0.5,-0.5) = bottom-left
    // (0.0, 0.5)  = top-center
    // (0.5,-0.5)  = bottom-right
    // ======================================================
    float vertices[] = {

        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // ======================================================
    // Vertex Buffer Object (VBO)
    //
    // Stores vertex data inside GPU memory
    // ======================================================
    unsigned int VBO;

    // Generate one buffer object
    glGenBuffers(1, &VBO);

    // Bind buffer as active GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertex data into GPU memory
    glBufferData(
        GL_ARRAY_BUFFER,     // Buffer type
        sizeof(vertices),    // Size in bytes
        vertices,            // Vertex data
        GL_STATIC_DRAW       // Data usage type
    );

    // ======================================================
    // Vertex Array Object (VAO)
    //
    // Stores vertex attribute configuration
    // ======================================================
    unsigned int VAO;

    // Generate VAO
    glGenVertexArrays(1, &VAO);

    // Bind VAO
    glBindVertexArray(VAO);

    // ======================================================
    // Bind VBO again while VAO is active
    //
    // This lets the VAO remember the VBO settings
    // ======================================================
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // ======================================================
    // Define vertex layout
    //
    // Parameters:
    //
    // 0 = shader attribute location
    // 2 = x,y values
    // GL_FLOAT = float type
    // GL_FALSE = don't normalize
    // stride = size of one vertex
    // offset = where data starts
    // ======================================================
    glVertexAttribPointer(

        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    // ======================================================
    // Enable attribute location 0
    // ======================================================
    glEnableVertexAttribArray(0);

    // ======================================================
    // Vertex Shader Source Code
    //
    // Runs once for every vertex
    // ======================================================
    const char* vertexShaderSource =

        // OpenGL shader version
        "#version 330 core\n"

        // Input variable at location 0
        "layout (location = 0) in vec2 aPos;\n"

        // Main shader function
        "void main()\n"

        "{\n"

        // Convert 2D position into 4D clip-space position
        "   gl_Position = vec4(aPos, 0.0, 1.0);\n"

        "}\0";

    // ======================================================
    // Fragment Shader Source Code
    //
    // Runs once per pixel
    // ======================================================
    const char* fragmentShaderSource =

        "#version 330 core\n"

        // Output color
        "out vec4 FragColor;\n"

        "void main()\n"

        "{\n"

        // Red color (R,G,B,A)
        "   FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"

        "}\0";

    // ======================================================
    // Create Vertex Shader
    // ======================================================
    unsigned int vertexShader;

    // Create shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach shader source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compile shader
    glCompileShader(vertexShader);

    // ======================================================
    // Create Fragment Shader
    // ======================================================
    unsigned int fragmentShader;

    // Create shader object
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach shader source
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compile shader
    glCompileShader(fragmentShader);

    // ======================================================
    // Shader Program
    //
    // Combines vertex + fragment shaders
    // ======================================================
    unsigned int shaderProgram;

    // Create program object
    shaderProgram = glCreateProgram();

    // Attach vertex shader
    glAttachShader(shaderProgram, vertexShader);

    // Attach fragment shader
    glAttachShader(shaderProgram, fragmentShader);

    // Link shaders together
    glLinkProgram(shaderProgram);

    // ======================================================
    // Delete standalone shaders
    //
    // Program already contains them
    // ======================================================
    glDeleteShader(vertexShader);

    glDeleteShader(fragmentShader);

    // ======================================================
    // Main render loop
    //
    // Runs until window closes
    // ======================================================
    while (!glfwWindowShouldClose(window))
    {
        // ==================================================
        // Clear screen
        // ==================================================
        glClear(GL_COLOR_BUFFER_BIT);

        // ==================================================
        // Activate shader program
        // ==================================================
        glUseProgram(shaderProgram);

        // ==================================================
        // Bind VAO
        // ==================================================
        glBindVertexArray(VAO);

        // ==================================================
        // Draw triangle
        //
        // Parameters:
        // GL_TRIANGLES = drawing mode
        // 0 = starting vertex
        // 3 = number of vertices
        // ==================================================
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ==================================================
        // Swap front/back buffers
        //
        // Displays rendered image
        // ==================================================
        glfwSwapBuffers(window);

        // ==================================================
        // Process input/events
        // ==================================================
        glfwPollEvents();
    }

    // ======================================================
    // Cleanup GPU resources
    // ======================================================

    // Delete VAO
    glDeleteVertexArrays(1, &VAO);

    // Delete VBO
    glDeleteBuffers(1, &VBO);

    // Delete shader program
    glDeleteProgram(shaderProgram);

    // ======================================================
    // Shutdown GLFW
    // ======================================================
    glfwTerminate();

    // Program ended successfully
    return 0;
}