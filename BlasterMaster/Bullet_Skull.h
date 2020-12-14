#pragma once
#include "Bullet.h"
class CBullet_Skull : public CBullet
{
private:
    const float TIMETOEXPLODE = 2000;
    DWORD timestartDrop;
public:
    const float BULLET_SKULL_SPEED = 0.3f;

    CBullet_Skull() {};
    CBullet_Skull(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    //void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};
