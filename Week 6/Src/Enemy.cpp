#include "Enemy.h"

#include <cmath>
#include <cstdlib>

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
        350.0f;

    shootingRange =
        250.0f;

    fireCooldown =
        1.2f;

    fireTimer =
        0.0f;

    state =
        EnemyState::Patrol;

    patrolDirection =
    {
        1.0f,
        0.0f
    };

    patrolTimer =
        0.0f;

    patrolDuration =
        2.0f;

    moveSpeed =
        100.0f;

    rotationSpeed =
        2.5f;

    ChooseNewPatrolDirection();
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
// Helpers
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

EnemyState Enemy::GetState() const
{
    return state;
}

//--------------------------------------------------
// Patrol
//--------------------------------------------------

void Enemy::ChooseNewPatrolDirection()
{
    int random =
        std::rand() % 4;

    switch (random)
    {
    case 0:
        patrolDirection =
        {
            1.0f,
            0.0f
        };
        break;

    case 1:
        patrolDirection =
        {
            -1.0f,
            0.0f
        };
        break;

    case 2:
        patrolDirection =
        {
            0.0f,
            1.0f
        };
        break;

    default:
        patrolDirection =
        {
            0.0f,
            -1.0f
        };
        break;
    }

    patrolDuration =
        1.5f +
        static_cast<float>(
            std::rand() % 300)
        / 100.0f;
}

//--------------------------------------------------
// Update
//--------------------------------------------------

void Enemy::Update(
    float deltaTime)
{
    if (!alive)
    {
        return;
    }

    fireTimer +=
        deltaTime;

    //--------------------------------------------------
    // State Change
    //--------------------------------------------------

    if (CanSeeTarget())
    {
        state =
            EnemyState::Chase;
    }
    else
    {
        state =
            EnemyState::Patrol;
    }

    //--------------------------------------------------
    // Chase
    //--------------------------------------------------

    if (
        state ==
        EnemyState::Chase)
    {
        glm::vec2 direction =
            DirectionToTarget();

        if (
            glm::dot(
                direction,
                direction)
        >
            0.0001f)
        {
            rotation =
                std::atan2(
                    direction.y,
                    direction.x);

            position +=
                direction *
                moveSpeed *
                deltaTime;
        }

        return;
    }

    //--------------------------------------------------
    // Patrol
    //--------------------------------------------------

    patrolTimer +=
        deltaTime;

    if (
        patrolTimer >=
        patrolDuration)
    {
        patrolTimer =
            0.0f;

        ChooseNewPatrolDirection();
    }

    rotation =
        std::atan2(
            patrolDirection.y,
            patrolDirection.x);

    position +=
        patrolDirection *
        moveSpeed *
        deltaTime;

    //--------------------------------------------------
    // Map Limits
    //--------------------------------------------------

    if (position.x < 32.0f)
    {
        position.x = 32.0f;
        ChooseNewPatrolDirection();
    }

    if (position.x > 1248.0f)
    {
        position.x = 1248.0f;
        ChooseNewPatrolDirection();
    }

    if (position.y < 32.0f)
    {
        position.y = 32.0f;
        ChooseNewPatrolDirection();
    }

    if (position.y > 688.0f)
    {
        position.y = 688.0f;
        ChooseNewPatrolDirection();
    }
}

//--------------------------------------------------
// Destructor
//--------------------------------------------------

Enemy::~Enemy()
{
}