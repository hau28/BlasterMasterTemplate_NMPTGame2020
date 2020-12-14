#pragma once
#include "Bullet.h"
#include "Timer.h"

class CBullet_Skull : public CBullet, public ITimeTrackable
{
private:
    const float TIME_TO_EXPLODE = 2000;

    const float BULLET_SKULL_GRAVITY = 0.02f;
    const float BULLET_SKULL_FRICTION = 0.04f;

    CTimer* explodeTimer = nullptr;

public:
    const float BULLET_SKULL_SPEED = 0.3f;

    CBullet_Skull() {};
    CBullet_Skull(float x, float y, int section, float dirX, float dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void HandleTimerTick(CTimer* sender);

    ~CBullet_Skull();
    //void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};
