#pragma once
#include "Enemy.h"

// bounding box
// 8 8 23 23

const int DOME_BOUNDBOX_WIDTH = 15;
const int DOME_BOUNDBOX_HEIGHT = 15;
const int DOME_BOUNDBOX_OFFSETX = 8;
const int DOME_BOUNDBOX_OFFSETY = 8;

class CDome : public CEnemy
{
public:
    CDome() {};
    CDome(int classId, int x, int y, int animsId);
    
    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) {};
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CDome* LPDOME;
