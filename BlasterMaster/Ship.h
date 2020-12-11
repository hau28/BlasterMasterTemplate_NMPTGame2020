#pragma once
#include "Enemy.h"
#include "Timer.h"

// 5 9 26 24

const int SHIP_BOUNDBOX_WIDTH = 22;
const int SHIP_BOUNDBOX_HEIGHT = 15;
const int SHIP_BOUNDBOX_OFFSETX = 5;
const int SHIP_BOUNDBOX_OFFSETY = 9;

const float SHIP_MOVE_SPEED = 0.04f;

class CShip : public CEnemy, public ITimeTrackable
{
private:
    const int SHOOT_PER_SHOOTING_PHASE = 4;

    void UpdateState();

    int shootPhase;
    

    LPTIMER singleShootTimer;
    void Shoot() { DebugOut(L"\nhihi %d\n"); }

public:
    CShip() {};
    CShip(int classId, int x, int y, int sectionId, int initLeft, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void HandleTimerTick(LPTIMER sender);
};

typedef CShip* LPSHIP;

