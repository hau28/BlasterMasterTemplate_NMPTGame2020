#pragma once
#include "Bullet.h"
class CBullet_Floater : public CBullet
{
public:
    const float BULLET_FLOATER_SPEED = 0.2f;

    CBullet_Floater() {};
    CBullet_Floater(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
};

