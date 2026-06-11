#include "Bullet.h"

#include <glm.hpp>

Bullet::Bullet()
{
    position =
    {
        0.0f,
        0.0f
    };

    direction =
    {
        1.0f,
        0.0f
    };

    speed =
        500.0f;

    width =
        8.0f;

    height =
        8.0f;

    fromPlayer =
        true;

    active =
        true;
}

//--------------------------------------------------
// Position
//--------------------------------------------------

void Bullet::SetPosition(
    const glm::vec2& newPosition)
{
    position =
        newPosition;
}

glm::vec2 Bullet::GetPosition() const
{
    return position;
}

//--------------------------------------------------
// Direction
//--------------------------------------------------

void Bullet::SetDirection(
    const glm::vec2& newDirection)
{
    if (
        glm::dot(
            newDirection,
            newDirection)
        >
        0.0001f)
    {
        direction =
            glm::normalize(
                newDirection);
    }
}

glm::vec2 Bullet::GetDirection() const
{
    return direction;
}

//--------------------------------------------------
// Speed
//--------------------------------------------------

void Bullet::SetSpeed(
    float newSpeed)
{
    speed =
        newSpeed;
}

float Bullet::GetSpeed() const
{
    return speed;
}

//--------------------------------------------------
// Size
//--------------------------------------------------

void Bullet::SetSize(
    float newWidth,
    float newHeight)
{
    width =
        newWidth;

    height =
        newHeight;
}

float Bullet::GetWidth() const
{
    return width;
}

float Bullet::GetHeight() const
{
    return height;
}

//--------------------------------------------------
// Ownership
//--------------------------------------------------

void Bullet::SetFromPlayer(
    bool value)
{
    fromPlayer =
        value;
}

bool Bullet::IsFromPlayer() const
{
    return fromPlayer;
}

//--------------------------------------------------
// State
//--------------------------------------------------

bool Bullet::IsActive() const
{
    return active;
}

void Bullet::Deactivate()
{
    active =
        false;
}

//--------------------------------------------------
// Bounds
//--------------------------------------------------

glm::vec2 Bullet::GetMinBounds() const
{
    return
    {
        position.x -
        width * 0.5f,

        position.y -
        height * 0.5f
    };
}

glm::vec2 Bullet::GetMaxBounds() const
{
    return
    {
        position.x +
        width * 0.5f,

        position.y +
        height * 0.5f
    };
}

//--------------------------------------------------
// Update
//--------------------------------------------------

void Bullet::Update(
    float deltaTime)
{
    position +=
        direction *
        speed *
        deltaTime;
}

//--------------------------------------------------
// Screen Test
//--------------------------------------------------

bool Bullet::IsOutsideScreen(
    int screenWidth,
    int screenHeight) const
{
    if (position.x < 0.0f)
        return true;

    if (position.y < 0.0f)
        return true;

    if (
        position.x >
        static_cast<float>(
            screenWidth))
    {
        return true;
    }

    if (
        position.y >
        static_cast<float>(
            screenHeight))
    {
        return true;
    }

    return false;
}

//--------------------------------------------------
// Destructor
//--------------------------------------------------

Bullet::~Bullet()
{
}