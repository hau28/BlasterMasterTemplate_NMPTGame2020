#pragma once
#include "Enemy.h"

class CMine : public CEnemy
{
    const int MINE_HEALTHPOINT = 1;

    const int MINE_BOUNDBOX_WIDTH = 14;
    const int MINE_BOUNDBOX_HEIGHT = 8;
    const int MINE_BOUNDBOX_OFFSETX = 1;
    const int MINE_BOUNDBOX_OFFSETY = 1;

    const float MINE_GRAVITY = 10.08; // Thycute's birthday -> not magic number

public:
    CMine() {};
    CMine(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CMine* LPMINE;
