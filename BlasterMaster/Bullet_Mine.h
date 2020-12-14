#pragma once
#include "Bullet.h"
class CBullet_Mine : public CBullet
{
private:
    const float BASEVELOCITY = 0.15f;
    
    float vx_factor ,vy_factor  ;
    DWORD timestartDrop;

public:
    const float BULLET_MINE_SPEED = 0.3f;

    CBullet_Mine() {};
    CBullet_Mine(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
  
};
