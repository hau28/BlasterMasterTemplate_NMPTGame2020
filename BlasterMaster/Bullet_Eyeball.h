#pragma once
#include "Bullet.h"
class CBullet_Eyeball : public CBullet
{
public:
    const float BULLET_EYEBALL_SPEED = 0.1f;

    CBullet_Eyeball() {};
    CBullet_Eyeball(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
};

