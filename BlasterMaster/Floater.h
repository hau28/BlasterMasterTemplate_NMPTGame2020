#pragma once
#include "Enemy.h"

class CFloater : public CEnemy
{
public:
    CFloater() {};
    CFloater(int classId, int x, int y, int animsId);
    
    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CFloater* LPFLOATER;
