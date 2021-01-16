#pragma once
#include "Enemy.h"
class CBoss : public CEnemy
{
    const int PANDA_HEALTHPOINT = 4;

    bool flagDead = false;
    bool flagOnAir = true;
    bool flagTouchWall = false;
    float destinationX;
    float flagPlayerX = 0;
    bool isGoingToPlayer = true;

public:
    CBoss() {};
    CBoss(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};

typedef CBoss* LPBOSS;