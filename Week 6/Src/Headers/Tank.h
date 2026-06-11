#ifndef TANK_H
#define TANK_H

#include <glm.hpp>

class Tank
{
protected:

    //--------------------------------------------------
    // Transform
    //--------------------------------------------------

    glm::vec2 position;

    float rotation;

    //--------------------------------------------------
    // Movement
    //--------------------------------------------------

    float moveSpeed;

    float rotationSpeed;

    //--------------------------------------------------
    // Collision
    //--------------------------------------------------

    float width;

    float height;

    //--------------------------------------------------
    // State
    //--------------------------------------------------

    bool alive;

public:

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Tank();

    //--------------------------------------------------
    // Position
    //--------------------------------------------------

    void SetPosition(
        const glm::vec2& newPosition);

    glm::vec2 GetPosition() const;

    //--------------------------------------------------
    // Rotation
    //--------------------------------------------------

    void SetRotation(
        float angle);

    float GetRotation() const;

    //--------------------------------------------------
    // Movement
    //--------------------------------------------------

    void MoveForward(
        float deltaTime);

    void MoveBackward(
        float deltaTime);

    void RotateLeft(
        float deltaTime);

    void RotateRight(
        float deltaTime);

    glm::vec2 GetForwardVector() const;

    //--------------------------------------------------
    // Speed
    //--------------------------------------------------

    void SetMoveSpeed(
        float speed);

    float GetMoveSpeed() const;

    void SetRotationSpeed(
        float speed);

    float GetRotationSpeed() const;

    //--------------------------------------------------
    // Size
    //--------------------------------------------------

    void SetSize(
        float tankWidth,
        float tankHeight);

    float GetWidth() const;

    float GetHeight() const;

    //--------------------------------------------------
    // Bounds
    //--------------------------------------------------

    glm::vec2 GetMinBounds() const;

    glm::vec2 GetMaxBounds() const;

    //--------------------------------------------------
    // State
    //--------------------------------------------------

    bool IsAlive() const;

    void Destroy();

    //--------------------------------------------------
    // Update
    //--------------------------------------------------

    virtual void Update(
        float deltaTime);

    //--------------------------------------------------
    // Destructor
    //--------------------------------------------------

    virtual ~Tank();
};

#endif