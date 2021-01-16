#pragma once
#include "Bullet.h"
class CBullet_Teleporter : public CBullet
{
public:
    const float BULLET_TELEPORTER_SPEED = 0.1f;

    CBullet_Teleporter() {};
    CBullet_Teleporter(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
};

