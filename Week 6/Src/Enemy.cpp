#include "Enemy.h"

#include <cmath>

#include <glm.hpp>

Enemy::Enemy()
    : Tank()
{
    targetPosition =
    {
        0.0f,
        0.0f
    };

    detectionRange =
        500.0f;

    shootingRange =
        300.0f;

    fireCooldown =
        1.5f;

    fireTimer =
        0.0f;

    moveSpeed =
        120.0f;

    rotationSpeed =
        2.5f;
}

//--------------------------------------------------
// Target
//--------------------------------------------------

void Enemy::SetTargetPosition(
    const glm::vec2& position)
{
    targetPosition =
        position;
}

glm::vec2 Enemy::GetTargetPosition() const
{
    return targetPosition;
}

//--------------------------------------------------
// Detection
//--------------------------------------------------

void Enemy::SetDetectionRange(
    float range)
{
    detectionRange =
        range;
}

float Enemy::GetDetectionRange() const
{
    return detectionRange;
}

//--------------------------------------------------
// Shooting
//--------------------------------------------------

void Enemy::SetShootingRange(
    float range)
{
    shootingRange =
        range;
}

float Enemy::GetShootingRange() const
{
    return shootingRange;
}

void Enemy::SetFireCooldown(
    float cooldown)
{
    fireCooldown =
        cooldown;
}

float Enemy::GetFireCooldown() const
{
    return fireCooldown;
}

bool Enemy::CanShoot() const
{
    return
        fireTimer >=
        fireCooldown;
}

void Enemy::ResetFireTimer()
{
    fireTimer = 0.0f;
}

//--------------------------------------------------
// AI Helpers
//--------------------------------------------------

float Enemy::DistanceToTarget() const
{
    return glm::distance(
        position,
        targetPosition);
}

glm::vec2 Enemy::DirectionToTarget() const
{
    glm::vec2 direction =
        targetPosition -
        position;

    if (
        glm::dot(
            direction,
            direction)
        <
        0.0001f)
    {
        return
        {
            0.0f,
            0.0f
        };
    }

    return glm::normalize(
        direction);
}

bool Enemy::CanSeeTarget() const
{
    return
        DistanceToTarget()
        <=
        detectionRange;
}

//--------------------------------------------------
// Update
//--------------------------------------------------

void Enemy::Update(
    float deltaTime)
{
    fireTimer +=
        deltaTime;

    if (!alive)
    {
        return;
    }

    if (!CanSeeTarget())
    {
        return;
    }

    glm::vec2 direction =
        DirectionToTarget();

    if (
        glm::dot(
            direction,
            direction)
        <
        0.0001f)
    {
        return;
    }

    rotation =
        std::atan2(
            direction.y,
            direction.x);

    position +=
        direction *
        moveSpeed *
        deltaTime;
}

//--------------------------------------------------
// Destructor
//--------------------------------------------------

Enemy::~Enemy()
{
}