#pragma once
#include "Enemy.h"

// 5 9 26 24

const int SHIP_BOUNDBOX_WIDTH = 22;
const int SHIP_BOUNDBOX_HEIGHT = 15;
const int SHIP_BOUNDBOX_OFFSETX = 5;
const int SHIP_BOUNDBOX_OFFSETY = 9;

const float SHIP_MOVE_SPEED = 0.04f;

class CShip : public CEnemy
{
private:
    void UpdateState();

public:
    CShip() {};
    CShip(int classId, int x, int y, int initLeft, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CShip* LPSHIP;

