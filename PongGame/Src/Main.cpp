// ==========================================================
// Pong Game With Score System
// OpenGL + GLFW + GLEW
// First to 10 wins
// OOP Version
// Author: Leonardo Moura
// Last Updated: 5/10/2026
// ==========================================================

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>

using namespace std;

// ==========================================================
// Window
// ==========================================================
const int WIDTH = 800;
const int HEIGHT = 600;

// ==========================================================
// Vertex Shader
// ==========================================================
const char* vertexShaderSource = R"(

#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 offset;
uniform vec2 scale;

void main()
{
    vec2 pos = aPos * scale + offset;

    gl_Position = vec4(pos, 0.0, 1.0);
}

)";

// ==========================================================
// Fragment Shader
// ==========================================================
const char* fragmentShaderSource = R"(

#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0);
}

)";

// ==========================================================
// Shader Creation
// ==========================================================
unsigned int createShader(unsigned int type,
    const char* source)
{
    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);

    glCompileShader(shader);

    return shader;
}

// ==========================================================
// Paddle Class
// ==========================================================
class Paddle
{
public:

    float x;
    float y;

    float width;
    float height;

    float speed;

    Paddle(float startX)
    {
        x = startX;
        y = 0.0f;

        width = 0.03f;
        height = 0.25f;

        speed = 0.02f;
    }

    void moveUp()
    {
        y += speed;
    }

    void moveDown()
    {
        y -= speed;
    }

    void clamp()
    {
        float halfHeight = height * 0.5f;

        if (y + halfHeight > 1.0f)
            y = 1.0f - halfHeight;

        if (y - halfHeight < -1.0f)
            y = -1.0f + halfHeight;
    }

    void reset()
    {
        y = 0.0f;
    }
};

// ==========================================================
// Ball Class
// ==========================================================
class Ball
{
public:

    float x;
    float y;

    float size;

    float speedX;
    float speedY;

    Ball()
    {
        x = 0.0f;
        y = 0.0f;

        size = 0.03f;

        speedX = 0.01f;
        speedY = 0.008f;
    }

    void move()
    {
        x += speedX;
        y += speedY;
    }

    void reset()
    {
        x = 0.0f;
        y = 0.0f;

        speedX *= -1.0f;
    }

    void checkWallCollision()
    {
        float halfSize = size * 0.5f;

        // Top
        if (y + halfSize > 1.0f)
        {
            y = 1.0f - halfSize;

            speedY *= -1.0f;
        }

        // Bottom
        if (y - halfSize < -1.0f)
        {
            y = -1.0f + halfSize;

            speedY *= -1.0f;
        }
    }
};

// ==========================================================
// Game Class
// ==========================================================
class Game
{
private:

    GLFWwindow* window;

    Paddle leftPaddle;
    Paddle rightPaddle;

    Ball ball;

    int leftScore;
    int rightScore;

    bool gameOver;

    string winnerText;

    unsigned int VAO;
    unsigned int VBO;

    unsigned int shaderProgram;

    int offsetLoc;
    int scaleLoc;
    int colorLoc;

public:

    Game()
        : leftPaddle(-0.9f),
        rightPaddle(0.9f)
    {
        leftScore = 0;
        rightScore = 0;

        gameOver = false;

        winnerText = "";
    }

    // ======================================================
    // Initialize
    // ======================================================
    bool initialize()
    {
        if (!glfwInit())
            return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_PROFILE,
            GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(
            WIDTH,
            HEIGHT,
            "Pong",
            NULL,
            NULL
        );

        if (!window)
        {
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
            cout << "GLEW failed\n";
            return false;
        }

        cout << "OpenGL Version: "
            << glGetString(GL_VERSION)
            << endl;

        setupBuffers();

        setupShaders();

        return true;
    }

    // ======================================================
    // Setup Buffers
    // ======================================================
    void setupBuffers()
    {
        float vertices[] =
        {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,

            -0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
        };

        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

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

    // ======================================================
    // Setup Shaders
    // ======================================================
    void setupShaders()
    {
        unsigned int vertexShader =
            createShader(GL_VERTEX_SHADER,
                vertexShaderSource);

        unsigned int fragmentShader =
            createShader(GL_FRAGMENT_SHADER,
                fragmentShaderSource);

        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);

        glAttachShader(shaderProgram, fragmentShader);

        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);

        glDeleteShader(fragmentShader);

        offsetLoc =
            glGetUniformLocation(shaderProgram,
                "offset");

        scaleLoc =
            glGetUniformLocation(shaderProgram,
                "scale");

        colorLoc =
            glGetUniformLocation(shaderProgram,
                "color");
    }

    // ======================================================
    // Restart Game
    // ======================================================
    void restartGame()
    {
        leftScore = 0;
        rightScore = 0;

        gameOver = false;

        winnerText = "";

        leftPaddle.reset();
        rightPaddle.reset();

        ball.reset();
    }

    // ======================================================
    // Update Window Title
    // ======================================================
    void updateWindowTitle()
    {
        string title;

        if (!gameOver)
        {
            title =
                "Pong  |  Left: " +
                to_string(leftScore) +
                "   Right: " +
                to_string(rightScore);
        }
        else
        {
            title =
                winnerText +
                " Wins!  |  ENTER = Restart  |  ESC = Exit";
        }

        glfwSetWindowTitle(window, title.c_str());
    }

    // ======================================================
    // Input
    // ======================================================
    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)
            == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        if (gameOver &&
            glfwGetKey(window, GLFW_KEY_ENTER)
            == GLFW_PRESS)
        {
            restartGame();
        }

        if (gameOver)
            return;

        // Left paddle
        if (glfwGetKey(window, GLFW_KEY_W)
            == GLFW_PRESS)
        {
            leftPaddle.moveUp();
        }

        if (glfwGetKey(window, GLFW_KEY_S)
            == GLFW_PRESS)
        {
            leftPaddle.moveDown();
        }

        // Right paddle
        if (glfwGetKey(window, GLFW_KEY_UP)
            == GLFW_PRESS)
        {
            rightPaddle.moveUp();
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN)
            == GLFW_PRESS)
        {
            rightPaddle.moveDown();
        }

        leftPaddle.clamp();
        rightPaddle.clamp();
    }

    // ======================================================
    // Ball + Collision Logic
    // ======================================================
    void update()
    {
        if (gameOver)
            return;

        float paddleHalfWidth =
            leftPaddle.width * 0.5f;

        float paddleHalfHeight =
            leftPaddle.height * 0.5f;

        float ballHalfSize =
            ball.size * 0.5f;

        ball.move();

        ball.checkWallCollision();

        // ==================================================
        // LEFT paddle collision
        // ==================================================
        bool collisionLeft =

            ball.x - ballHalfSize <
            leftPaddle.x + paddleHalfWidth &&

            ball.x + ballHalfSize >
            leftPaddle.x - paddleHalfWidth &&

            ball.y - ballHalfSize <
            leftPaddle.y + paddleHalfHeight &&

            ball.y + ballHalfSize >
            leftPaddle.y - paddleHalfHeight;

        if (collisionLeft && ball.speedX < 0.0f)
        {
            ball.speedX *= -1.0f;

            ball.x =
                leftPaddle.x +
                paddleHalfWidth +
                ballHalfSize;
        }

        // ==================================================
        // RIGHT paddle collision
        // ==================================================
        bool collisionRight =

            ball.x + ballHalfSize >
            rightPaddle.x - paddleHalfWidth &&

            ball.x - ballHalfSize <
            rightPaddle.x + paddleHalfWidth &&

            ball.y - ballHalfSize <
            rightPaddle.y + paddleHalfHeight &&

            ball.y + ballHalfSize >
            rightPaddle.y - paddleHalfHeight;

        if (collisionRight && ball.speedX > 0.0f)
        {
            ball.speedX *= -1.0f;

            ball.x =
                rightPaddle.x -
                paddleHalfWidth -
                ballHalfSize;
        }

        // ==================================================
        // LEFT scores
        // ==================================================
        if (ball.x > 1.1f)
        {
            leftScore++;

            ball.reset();
        }

        // ==================================================
        // RIGHT scores
        // ==================================================
        if (ball.x < -1.1f)
        {
            rightScore++;

            ball.reset();
        }

        // ==================================================
        // Win check
        // ==================================================
        if (leftScore >= 10)
        {
            gameOver = true;

            winnerText = "LEFT PLAYER";
        }

        if (rightScore >= 10)
        {
            gameOver = true;

            winnerText = "RIGHT PLAYER";
        }
    }

    // ======================================================
    // Draw Object
    // ======================================================
    void drawObject(float x,
        float y,
        float width,
        float height,
        float r,
        float g,
        float b)
    {
        glUniform2f(offsetLoc, x, y);

        glUniform2f(scaleLoc, width, height);

        glUniform3f(colorLoc, r, g, b);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // ======================================================
    // Render
    // ======================================================
    void render()
    {
        glClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);

        // Left paddle
        drawObject(
            leftPaddle.x,
            leftPaddle.y,
            leftPaddle.width,
            leftPaddle.height,
            1.0f,
            1.0f,
            1.0f
        );

        // Right paddle
        drawObject(
            rightPaddle.x,
            rightPaddle.y,
            rightPaddle.width,
            rightPaddle.height,
            1.0f,
            1.0f,
            1.0f
        );

        // Ball
        drawObject(
            ball.x,
            ball.y,
            ball.size,
            ball.size,
            1.0f,
            0.2f,
            0.2f
        );

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // ======================================================
    // Game Loop
    // ======================================================
    void run()
    {
        while (!glfwWindowShouldClose(window))
        {
            processInput();

            updateWindowTitle();

            update();

            render();
        }
    }

    // ======================================================
    // Cleanup
    // ======================================================
    void cleanup()
    {
        glDeleteVertexArrays(1, &VAO);

        glDeleteBuffers(1, &VBO);

        glDeleteProgram(shaderProgram);

        glfwTerminate();
    }
};

// ==========================================================
// Main
// ==========================================================
int main()
{
    Game game;

    if (!game.initialize())
        return -1;

    game.run();

    game.cleanup();

    return 0;
}