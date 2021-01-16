#pragma once
#include "Bullet.h"
class CBullet_Cannon : public CBullet
{
public:
    const float BULLET_CANNON_SPEED = 0.1f;

    CBullet_Cannon() {};
    CBullet_Cannon(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
};

