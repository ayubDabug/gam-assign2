// ======================================================
// GALAGA PROTOTYPE
// Modern OpenGL + OOP
//
// Features:
// - OpenGL 3.3 Core
// - VAO / VBO
// - GLSL shaders
// - Alien fleet
// - Collision detection
// - Win message on screen title
// - Restart system
//
// Controls:
// LEFT / RIGHT  -> Move
// SPACE         -> Shoot
// ENTER         -> Restart
// ESC           -> Exit
//
// Author: Leonardo Moura
// Date: May 2026
// ======================================================

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

// ======================================================
// WINDOW
// ======================================================

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// ======================================================
// OPENGL OBJECTS
// ======================================================

GLuint shaderProgram;

GLuint shipVAO;
GLuint shipVBO;

GLuint bulletVAO;
GLuint bulletVBO;

GLuint alienVAO;
GLuint alienVBO;

// ======================================================
// SHADERS
// ======================================================

const char* vertexShaderSource = R"(
    #version 330 core

    layout(location = 0) in vec2 aPos;

    uniform vec2 offset;

    void main()
    {
        gl_Position = vec4(aPos + offset, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core

    out vec4 FragColor;

    uniform vec3 objectColor;

    void main()
    {
        FragColor = vec4(objectColor, 1.0);
    }
)";

// ======================================================
// SHADER PROGRAM
// ======================================================

GLuint createShaderProgram()
{
    GLuint vertexShader =
        glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );

    glCompileShader(vertexShader);

    GLuint fragmentShader =
        glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );

    glCompileShader(fragmentShader);

    GLuint program =
        glCreateProgram();

    glAttachShader(program, vertexShader);

    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glDeleteShader(vertexShader);

    glDeleteShader(fragmentShader);

    return program;
}

// ======================================================
// BULLET
// ======================================================

class Bullet
{
    public:
        float x;
        float y;
        float speed;
        bool active;    

        Bullet(float startX, float startY)
        {
            x = startX;

            y = startY;

            speed = 0.004f;

            active = true;
        }

        void update()
        {
            if (!active)
                return;

            y += speed;

            if (y > 1.1f)
            {
                active = false;
            }
        }

        void draw()
        {
            if (!active)
                return;

            glUseProgram(shaderProgram);

            GLint offsetLoc =
                glGetUniformLocation(
                    shaderProgram,
                    "offset"
                );

            GLint colorLoc =
                glGetUniformLocation(
                    shaderProgram,
                    "objectColor"
                );

            glUniform2f(offsetLoc, x, y);

            glUniform3f(
                colorLoc,
                1.0f,
                1.0f,
                0.0f
            );

            glBindVertexArray(bulletVAO);

            glDrawArrays(GL_LINES, 0, 2);
        }
};

// ======================================================
// ALIEN
// ======================================================

class Alien
{
    public:

        float x;
        float y;
        float r;
        float g;
        float b;
        bool alive;

        Alien(
            float startX,
            float startY,
            float red,
            float green,
            float blue
        )
        {
            x = startX;

            y = startY;

            r = red;
            g = green;
            b = blue;

            alive = true;
        }

        void draw()
        {
            if (!alive)
                return;

            glUseProgram(shaderProgram);

            GLint offsetLoc =
                glGetUniformLocation(
                    shaderProgram,
                    "offset"
                );

            GLint colorLoc =
                glGetUniformLocation(
                    shaderProgram,
                    "objectColor"
                );

            glUniform2f(offsetLoc, x, y);

            glUniform3f(colorLoc, r, g, b);

            glBindVertexArray(alienVAO);

            glDrawArrays(GL_TRIANGLES, 0, 21);
        }
};

// ======================================================
// PLAYER SHIP
// ======================================================

class Spaceship
{
    private:
        float x;
        float y;
        float speed;

    public:

        Spaceship()
        {
            x = 0.0f;

            y = -0.80f;

            speed = 0.0025f;
        }

        void moveLeft()
        {
            x -= speed;

            if (x < -0.9f)
            {
                x = -0.9f;
            }
        }

        void moveRight()
        {
            x += speed;

            if (x > 0.9f)
            {
                x = 0.9f;
            }
        }

        Bullet shoot()
        {
            return Bullet(
                x,
                y + 0.05f
            );
        }

        void draw()
        {
            glUseProgram(shaderProgram);

            GLint offsetLoc =
                glGetUniformLocation(
                    shaderProgram,
                    "offset"
                );

            GLint colorLoc =
                glGetUniformLocation(
                    shaderProgram,
                    "objectColor"
                );

            glUniform2f(offsetLoc, x, y);

            glUniform3f(
                colorLoc,
                1.0f,
                0.0f,
                0.0f
            );

            glBindVertexArray(shipVAO);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
};

// ======================================================
// GAME
// ======================================================

class Game
{
    private:

        GLFWwindow* window;
        Spaceship ship;
        std::vector<Bullet> bullets;
        std::vector<Alien> aliens;
        bool spacePressed;
        bool enterPressed;
        bool gameWon;
        float alienDirection;

    public:
        Game()
        {
            window = nullptr;

            spacePressed = false;

            enterPressed = false;

            gameWon = false;

            alienDirection = 0.0005f;
        }

        // ==================================================
        // WINDOW TITLE
        // ==================================================

        void updateWindowTitle()
        {
            if (gameWon)
            {
                glfwSetWindowTitle(
                    window,
                    "YOU WIN! | ENTER = Restart | ESC = Exit"
                );
            }
            else
            {
                glfwSetWindowTitle(
                    window,
                    "Galaga Prototype"
                );
            }
        }

        // ==================================================
        // INITIALIZE
        // ==================================================

        bool initialize()
        {
            if (!glfwInit())
            {
                std::cout
                    << "Failed to initialize GLFW\n";

                return false;
            }

            glfwWindowHint(
                GLFW_CONTEXT_VERSION_MAJOR,
                3
            );

            glfwWindowHint(
                GLFW_CONTEXT_VERSION_MINOR,
                3
            );

            glfwWindowHint(
                GLFW_OPENGL_PROFILE,
                GLFW_OPENGL_CORE_PROFILE
            );

            window = glfwCreateWindow(
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                "Galaga Prototype",
                nullptr,
                nullptr
            );

            if (!window)
            {
                glfwTerminate();

                return false;
            }

            glfwMakeContextCurrent(window);

            glewExperimental = GL_TRUE;

            if (glewInit() != GLEW_OK)
            {
                return false;
            }

            glViewport(
                0,
                0,
                WINDOW_WIDTH,
                WINDOW_HEIGHT
            );

            shaderProgram =
                createShaderProgram();

            setupShip();

            setupBullet();

            setupAlien();

            createAlienFleet();

            updateWindowTitle();

            return true;
        }

        // ==================================================
        // SHIP GEOMETRY
        // ==================================================

        void setupShip()
        {
            float vertices[] =
            {
                 0.00f,  0.05f,
                -0.05f, -0.05f,
                 0.05f, -0.05f
            };

            glGenVertexArrays(1, &shipVAO);

            glGenBuffers(1, &shipVBO);

            glBindVertexArray(shipVAO);

            glBindBuffer(
                GL_ARRAY_BUFFER,
                shipVBO
            );

            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices,
                GL_STATIC_DRAW
            );

            glVertexAttribPointer(
                0,
                2,
                GL_FLOAT,
                GL_FALSE,
                2 * sizeof(float),
                (void*)0
            );

            glEnableVertexAttribArray(0);
        }

        // ==================================================
        // BULLET GEOMETRY
        // ==================================================

        void setupBullet()
        {
            float vertices[] =
            {
                0.0f, 0.0f,
                0.0f, 0.03f
            };

            glGenVertexArrays(1, &bulletVAO);

            glGenBuffers(1, &bulletVBO);

            glBindVertexArray(bulletVAO);

            glBindBuffer(
                GL_ARRAY_BUFFER,
                bulletVBO
            );

            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices,
                GL_STATIC_DRAW
            );

            glVertexAttribPointer(
                0,
                2,
                GL_FLOAT,
                GL_FALSE,
                2 * sizeof(float),
                (void*)0
            );

            glEnableVertexAttribArray(0);
        }

        // ==================================================
        // ALIEN GEOMETRY
        // ==================================================

        void setupAlien()
        {
            float vertices[] =
            {
                // TOP TRIANGLE

                 0.00f,  0.045f,
                -0.020f, 0.010f,
                 0.020f, 0.010f,

                 // LEFT TOP TRIANGLE

                 -0.022f,  0.015f,
                 -0.070f,  0.000f,
                 -0.035f, -0.020f,

                 // LEFT BOTTOM TRIANGLE

                 -0.035f, -0.020f,
                 -0.070f, -0.055f,
                 -0.022f, -0.065f,

                 // RIGHT TOP TRIANGLE

                  0.022f,  0.015f,
                  0.070f,  0.000f,
                  0.035f, -0.020f,

                  // RIGHT BOTTOM TRIANGLE

                   0.035f, -0.020f,
                   0.070f, -0.055f,
                   0.022f, -0.065f,

                   // RECTANGLE BODY TRIANGLE 1

                   -0.020f,  0.008f,
                    0.020f,  0.008f,
                    0.020f, -0.060f,

                    // RECTANGLE BODY TRIANGLE 2

                    -0.020f,  0.008f,
                     0.020f, -0.060f,
                    -0.020f, -0.060f
            };

            glGenVertexArrays(1, &alienVAO);

            glGenBuffers(1, &alienVBO);

            glBindVertexArray(alienVAO);

            glBindBuffer(
                GL_ARRAY_BUFFER,
                alienVBO
            );

            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices,
                GL_STATIC_DRAW
            );

            glVertexAttribPointer(
                0,
                2,
                GL_FLOAT,
                GL_FALSE,
                2 * sizeof(float),
                (void*)0
            );

            glEnableVertexAttribArray(0);
        }

        // ==================================================
        // CREATE FLEET
        // ==================================================

        void createAlienFleet()
        {
            aliens.clear();

            float startX8 = -0.60f;
            float spacing8 = 0.18f;

            float startX10 = -0.72f;
            float spacing10 = 0.16f;

            // FIRST 2 ROWS

            for (int row = 0; row < 2; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    float x =
                        startX8 +
                        col * spacing8;

                    float y =
                        0.82f -
                        row * 0.12f;

                    if (row == 0)
                    {
                        aliens.push_back(
                            Alien(
                                x,
                                y,
                                0.0f,
                                1.0f,
                                0.0f
                            )
                        );
                    }
                    else
                    {
                        aliens.push_back(
                            Alien(
                                x,
                                y,
                                0.0f,
                                0.5f,
                                1.0f
                            )
                        );
                    }
                }
            }

        // SECOND 2 ROWS

        for (int row = 0; row < 2; row++)
        {
            for (int col = 0; col < 10; col++)
            {
                float x =
                    startX10 +
                    col * spacing10;

                float y =
                    0.56f -
                    row * 0.12f;

                aliens.push_back(
                    Alien(
                        x,
                        y,
                        1.0f,
                        1.0f,
                        1.0f
                    )
                );
            }
        }
    }

        // ==================================================
        // RESET GAME
        // ==================================================

        void resetGame()
        {
            bullets.clear();

            createAlienFleet();

            alienDirection = 0.0005f;

            gameWon = false;

            updateWindowTitle();
        }

        // ==================================================
        // CHECK WIN
        // ==================================================

        bool allAliensDestroyed()
        {
            for (auto& alien : aliens)
            {
                if (alien.alive)
                {
                    return false;
                }
            }

            return true;
        }

        // ==================================================
        // INPUT
        // ==================================================

        void processInput()
        {
            if (gameWon)
            {
                if (glfwGetKey(
                    window,
                    GLFW_KEY_ENTER
                ) == GLFW_PRESS)
                {
                    if (!enterPressed)
                    {
                        resetGame();

                        enterPressed = true;
                    }
                }

                if (glfwGetKey(
                    window,
                    GLFW_KEY_ENTER
                ) == GLFW_RELEASE)
                {
                    enterPressed = false;
                }

                if (glfwGetKey(
                    window,
                    GLFW_KEY_ESCAPE
                ) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(
                        window,
                        true
                    );
                }

                return;
            }

            if (glfwGetKey(
                window,
                GLFW_KEY_LEFT
            ) == GLFW_PRESS)
            {
                ship.moveLeft();
            }

            if (glfwGetKey(
                window,
                GLFW_KEY_RIGHT
            ) == GLFW_PRESS)
            {
                ship.moveRight();
            }

            if (glfwGetKey(
                window,
                GLFW_KEY_SPACE
            ) == GLFW_PRESS)
            {
                if (!spacePressed)
                {
                    bullets.push_back(
                        ship.shoot()
                    );

                    spacePressed = true;
                }
            }

            if (glfwGetKey(
                window,
                GLFW_KEY_SPACE
            ) == GLFW_RELEASE)
            {
                spacePressed = false;
            }
        }

        // ==================================================
        // UPDATE ALIENS
        // ==================================================

        void updateAliens()
        {
            bool changeDirection = false;

            for (auto& alien : aliens)
            {
                if (!alien.alive)
                    continue;

                alien.x += alienDirection;

                if (alien.x > 0.88f ||
                    alien.x < -0.88f)
                {
                    changeDirection = true;
                }
            }

            if (changeDirection)
            {
                alienDirection *= -1.0f;
            }
        }

        // ==================================================
        // COLLISIONS
        // ==================================================

        void checkCollisions()
        {
            for (auto& bullet : bullets)
            {
                if (!bullet.active)
                    continue;

                for (auto& alien : aliens)
                {
                    if (!alien.alive)
                        continue;

                    float dx =
                        bullet.x - alien.x;

                    float dy =
                        bullet.y - alien.y;

                    float distance =
                        std::sqrt(
                            dx * dx +
                            dy * dy
                        );

                    if (distance < 0.06f)
                    {
                        alien.alive = false;

                        bullet.active = false;
                    }
                }
            }
        }

        // ==================================================
        // UPDATE
        // ==================================================

        void update()
        {
            if (gameWon)
                return;

            for (auto& bullet : bullets)
            {
                bullet.update();
            }

            updateAliens();

            checkCollisions();

            if (allAliensDestroyed())
            {
                gameWon = true;

                updateWindowTitle();
            }
        }

        // ==================================================
        // RENDER
        // ==================================================

        void render()
        {
            if (gameWon)
            {
                glClearColor(
                    0.0f,
                    0.2f,
                    0.0f,
                    1.0f
                );
            }
            else
            {
                glClearColor(
                    0.0f,
                    0.0f,
                    0.1f,
                    1.0f
                );
            }

            glClear(GL_COLOR_BUFFER_BIT);

            ship.draw();

            for (auto& bullet : bullets)
            {
                bullet.draw();
            }

            for (auto& alien : aliens)
            {
                alien.draw();
            }
        }

        // ==================================================
        // MAIN LOOP
        // ==================================================

        void run()
        {
            while (!glfwWindowShouldClose(window))
            {
                processInput();

                update();

                render();

                glfwSwapBuffers(window);

                glfwPollEvents();
            }
        }

        // ==================================================
        // CLEANUP
        // ==================================================

        void shutdown()
        {
            glDeleteVertexArrays(
                1,
                &shipVAO
            );

            glDeleteBuffers(
                1,
                &shipVBO
            );

            glDeleteVertexArrays(
                1,
                &bulletVAO
            );

            glDeleteBuffers(
                1,
                &bulletVBO
            );

            glDeleteVertexArrays(
                1,
                &alienVAO
            );

            glDeleteBuffers(
                1,
                &alienVBO
            );

            glDeleteProgram(shaderProgram);

            glfwTerminate();
        }
};

// ======================================================
// MAIN
// ======================================================

int main()
{
    Game game;

    if (!game.initialize())
    {
        return -1;
    }

    game.run();

    game.shutdown();

    return 0;
}