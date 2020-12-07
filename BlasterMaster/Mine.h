#pragma once
#include "Enemy.h"

class CMine : public CEnemy
{

    const int MINE_BOUNDBOX_WIDTH = 14;
    const int MINE_BOUNDBOX_HEIGHT = 8;
    const int MINE_BOUNDBOX_OFFSETX = 1;
    const int MINE_BOUNDBOX_OFFSETY = 1;


public:
    CMine() {};
    CMine(int classId, int x, int y, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CMine* LPMINE;
