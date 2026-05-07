// GLEW must be included BEFORE GLFW.
// Why?
// GLEW loads OpenGL function pointers and includes the correct OpenGL headers.
// GLFW may also include OpenGL headers internally.
//
// If GLFW is included first, you can get conflicts like:
// - redefinition errors
// - missing OpenGL symbols
// - incorrect function declarations
//
// GLEW and GLFW have different jobs:
//
// GLFW:
// - Creates the application window
// - Creates the OpenGL context
// - Handles keyboard and mouse input
// - Handles events and window management
//
// GLEW:
// - Loads modern OpenGL functions from the GPU driver
// - Gives access to advanced OpenGL features
// - Makes functions like glCreateShader(), glGenBuffers(), etc. available
//
// Why both are needed:
// GLFW creates the window and OpenGL context,
// while GLEW loads the OpenGL functions we use for rendering.
//
// Important:
// GLEW requires an active OpenGL context before glewInit() can work.
//
// Correct order:
// 1. Create window with GLFW
// 2. Make OpenGL context current
// 3. Initialize GLEW
//
// Rule of thumb:
// Include order should usually be:
// 1. GLEW
// 2. GLFW
// 3. Other headers
#include <glew.h>   
// GLEW must come first because it loads OpenGL function pointers
// and manages OpenGL extensions before other OpenGL headers
#include <glfw3.h>  // GLFW handles window creation, input, and OpenGL context management

#include <iostream> // Needed for cout and endl

using namespace std;

int main(void)
{
    GLFWwindow* window; // Pointer to a GLFW window object

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1; // Exit if GLFW initialization fails

    // Initialize GLEW
    // GLEW helps access modern OpenGL functions
    if (!glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(
        640,                       // Width
        480,                       // Height
        "Hello World here is OpenGL Window", // Window title
        NULL,                      // No monitor (windowed mode)
        NULL                       // No shared context
    );

    // Check if window creation failed
    if (!window)
    {
        glfwTerminate(); // Clean up GLFW
        return -1;
    }

    /* Make the window's OpenGL context current */
    glfwMakeContextCurrent(window);

    // Print the OpenGL version being used
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

    /* Main render loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear the screen */
        glClear(GL_COLOR_BUFFER_BIT);

        // =========================
        // Draw a square on the left
        // =========================
        glBegin(GL_QUADS);

            // Bottom-left
            glVertex2f(-0.9f, -0.3f);

            // Top-left
            glVertex2f(-0.9f, 0.3f);

            // Top-right
            glVertex2f(-0.3f, 0.3f);

            // Bottom-right
            glVertex2f(-0.3f, -0.3f);

        glEnd();


        // =========================
        // Draw a triangle in center
        // =========================
        glBegin(GL_TRIANGLES);

            // Bottom-left
            glVertex2f(-0.2f, -0.3f);

            // Top-center
            glVertex2f(0.0f, 0.3f);

            // Bottom-right
            glVertex2f(0.2f, -0.3f);

        glEnd();


        // =========================
        // Draw a circle on the right
        // =========================
        glBegin(GL_POLYGON);

            // Circle settings
            float radius = 0.3f;
            float centerX = 0.7f;
            float centerY = 0.0f;

            // Create many vertices to simulate a circle
            for (int i = 0; i < 360; i++)
            {
                float angle = i * 3.14159f / 180.0f;

                float x = radius * cos(angle);
                float y = radius * sin(angle);

                glVertex2f(x + centerX, y + centerY);
            }

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Process window events such as keyboard and mouse input */
        glfwPollEvents();
    }

    // Clean up and close GLFW
    glfwTerminate();

    return 0; // Program finished successfully
}