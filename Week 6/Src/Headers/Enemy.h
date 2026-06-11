#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.h"

#include <glm.hpp>

class Enemy : public Tank
{
private:

    //--------------------------------------------------
    // AI
    //--------------------------------------------------

    glm::vec2 targetPosition;

    float detectionRange;

    float shootingRange;

    //--------------------------------------------------
    // Shooting
    //--------------------------------------------------

    float fireCooldown;

    float fireTimer;

public:

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Enemy();

    //--------------------------------------------------
    // Target
    //--------------------------------------------------

    void SetTargetPosition(
        const glm::vec2& position);

    glm::vec2 GetTargetPosition() const;

    //--------------------------------------------------
    // Detection
    //--------------------------------------------------

    void SetDetectionRange(
        float range);

    float GetDetectionRange() const;

    //--------------------------------------------------
    // Shooting
    //--------------------------------------------------

    void SetShootingRange(
        float range);

    float GetShootingRange() const;

    void SetFireCooldown(
        float cooldown);

    float GetFireCooldown() const;

    bool CanShoot() const;

    void ResetFireTimer();

    //--------------------------------------------------
    // AI Helpers
    //--------------------------------------------------

    float DistanceToTarget() const;

    glm::vec2 DirectionToTarget() const;

    bool CanSeeTarget() const;

    //--------------------------------------------------
    // Update
    //--------------------------------------------------

    virtual void Update(
        float deltaTime) override;

    //--------------------------------------------------
    // Destructor
    //--------------------------------------------------

    virtual ~Enemy();
};

#endif