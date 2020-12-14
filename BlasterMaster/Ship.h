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
    const int SHOT_PER_SHOOTING_PHASE = 4;
    const int DELAY_BETWEEN_SHOOTING_PHASES = 2500;
    const int DELAY_BETWEEN_SHOTS = 300;

    const int SHIP_HEALTHPOINT = 10;

    void UpdateState();

    LPTIMER shootPhaseTimer;
    LPTIMER singleShotTimer;
    void ShootPlayer();

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

