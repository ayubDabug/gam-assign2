#include "Tank.h"

#include <cmath>

Tank::Tank()
{
    position =
    {
        0.0f,
        0.0f
    };

    rotation =
        0.0f;

    moveSpeed =
        220.0f;

    rotationSpeed =
        3.0f;

    width =
        32.0f;

    height =
        32.0f;

    alive =
        true;
}

//--------------------------------------------------
// Position
//--------------------------------------------------

void Tank::SetPosition(
    const glm::vec2& newPosition)
{
    position =
        newPosition;
}

glm::vec2 Tank::GetPosition() const
{
    return position;
}

//--------------------------------------------------
// Rotation
//--------------------------------------------------

void Tank::SetRotation(
    float angle)
{
    rotation =
        angle;
}

float Tank::GetRotation() const
{
    return rotation;
}

//--------------------------------------------------
// Movement
//--------------------------------------------------

glm::vec2 Tank::GetForwardVector() const
{
    return
    {
        std::cos(rotation),
        std::sin(rotation)
    };
}

void Tank::MoveForward(
    float deltaTime)
{
    position +=
        GetForwardVector()
        *
        moveSpeed
        *
        deltaTime;
}

void Tank::MoveBackward(
    float deltaTime)
{
    position -=
        GetForwardVector()
        *
        moveSpeed
        *
        deltaTime;
}

void Tank::RotateLeft(
    float deltaTime)
{
    rotation +=
        rotationSpeed
        *
        deltaTime;
}

void Tank::RotateRight(
    float deltaTime)
{
    rotation -=
        rotationSpeed
        *
        deltaTime;
}

//--------------------------------------------------
// Speed
//--------------------------------------------------

void Tank::SetMoveSpeed(
    float speed)
{
    moveSpeed =
        speed;
}

float Tank::GetMoveSpeed() const
{
    return moveSpeed;
}

void Tank::SetRotationSpeed(
    float speed)
{
    rotationSpeed =
        speed;
}

float Tank::GetRotationSpeed() const
{
    return rotationSpeed;
}

//--------------------------------------------------
// Size
//--------------------------------------------------

void Tank::SetSize(
    float tankWidth,
    float tankHeight)
{
    width =
        tankWidth;

    height =
        tankHeight;
}

float Tank::GetWidth() const
{
    return width;
}

float Tank::GetHeight() const
{
    return height;
}

//--------------------------------------------------
// Bounds
//--------------------------------------------------

glm::vec2 Tank::GetMinBounds() const
{
    return
    {
        position.x -
        width * 0.5f,

        position.y -
        height * 0.5f
    };
}

glm::vec2 Tank::GetMaxBounds() const
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
// State
//--------------------------------------------------

bool Tank::IsAlive() const
{
    return alive;
}

void Tank::Destroy()
{
    alive =
        false;
}

//--------------------------------------------------
// Update
//--------------------------------------------------

void Tank::Update(
    float deltaTime)
{
    (void)deltaTime;
}

//--------------------------------------------------
// Destructor
//--------------------------------------------------

Tank::~Tank()
{
}