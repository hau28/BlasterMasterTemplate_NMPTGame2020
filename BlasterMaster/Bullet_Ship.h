#pragma once
#include "Bullet.h"
class CBullet_Ship : public CBullet
{
public:
    const float BULLET_SHIP_SPEED = 0.3f;

	CBullet_Ship() {};
	CBullet_Ship(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
};

