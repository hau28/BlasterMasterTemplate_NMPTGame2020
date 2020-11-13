#pragma once
#include "Enemy.h"

// bound 8 12 23 18

class CDome : public CEnemy
{
public:
    CDome() {};
    CDome(int classId, int x, int y, int animsId);
    
    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CDome* LPDOME;
