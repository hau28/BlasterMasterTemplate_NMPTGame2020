#pragma once
#include "Enemy.h"

class CSkull : public CEnemy
{

    const int SKULL_BOUNDBOX_WIDTH = 18;
    const int SKULL_BOUNDBOX_HEIGHT =21 ;
    const int SKULL_BOUNDBOX_OFFSETX = 7;
    const int SKULL_BOUNDBOX_OFFSETY = 7;


public:
    CSkull() {};
    CSkull(int classId, int x, int y, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CSkull* LPSKULL;
