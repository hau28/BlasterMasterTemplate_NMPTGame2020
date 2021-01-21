#pragma once
#include "Enemy.h"
#include "Timer.h"


const int CANNON_BOUNDBOX_WIDTH = 20;
const int CANNON_BOUNDBOX_HEIGHT = 24;
const int CANNON_BOUNDBOX_OFFSETX = 6;
const int CANNON_BOUNDBOX_OFFSETY = 4;

const float CANNON_FLY_SPEED = 0.06f;
const float CANNON_BULLET_SPEED = 0.08f;

const int CANNON_HEALTHPOINT = 2;



class CCannon : public CEnemy, public ITimeTrackable
{
private:
    const int SHOT_PER_SHOOTING_PHASE = 2;
    const int DELAY_BETWEEN_SHOOTING_PHASES = 2000;
    const int DELAY_BETWEEN_SHOTS = 300;
    const int SHOOT_DURATION = 250;
    void UpdateState();
    bool flagVertical, flagHorizontal;
    void ShootVertical();
    void ShootHorizontal();

    virtual void DropItem();

public:
    CCannon() {};
    CCannon(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void HandleTimerTick(LPTIMER sender);
};

typedef CCannon* LPCANNON;
