#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"

#include <glm.hpp>

enum class EnemyState
{
    Patrol,
    Chase
};

class Enemy : public Tank
{
private:

    glm::vec2 targetPosition;

    float detectionRange;
    float shootingRange;

    float fireCooldown;
    float fireTimer;

    EnemyState state;

    glm::vec2 patrolDirection;

    float patrolTimer;
    float patrolDuration;

public:

    Enemy();

    void SetTargetPosition(
        const glm::vec2& position);

    glm::vec2 GetTargetPosition() const;

    void SetDetectionRange(
        float range);

    float GetDetectionRange() const;

    void SetShootingRange(
        float range);

    float GetShootingRange() const;

    void SetFireCooldown(
        float cooldown);

    float GetFireCooldown() const;

    bool CanShoot() const;

    void ResetFireTimer();

    float DistanceToTarget() const;

    glm::vec2 DirectionToTarget() const;

    bool CanSeeTarget() const;

    EnemyState GetState() const;

    void ChooseNewPatrolDirection();

    virtual void Update(
        float deltaTime) override;

    virtual ~Enemy();
};

#endif