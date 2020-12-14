#pragma once
#include "Enemy.h"
#include "Timer.h"

// bounding box
// 8 0 23 15

const int FLOATER_BOUNDBOX_WIDTH = 15;
const int FLOATER_BOUNDBOX_HEIGHT = 15;
const int FLOATER_BOUNDBOX_OFFSETX = 8;
const int FLOATER_BOUNDBOX_OFFSETY = 0;

const float FLOATER_FLY_SPEED = 0.06f;
const float FLOATER_BULLET_SPEED = 0.08f;



class CFloater : public CEnemy, public ITimeTrackable
{
private:
    const int FLOATER_HEALTHPOINT = 3;

    const int SHOT_PER_SHOOTING_PHASE = 2;
    const int DELAY_BETWEEN_SHOOTING_PHASES = 2000;
    const int DELAY_BETWEEN_SHOTS = 300;
    const int SHOOT_DURATION = 250;
    void UpdateState();

    LPTIMER shootPhaseTimer;
    LPTIMER singleShotTimer;
    LPTIMER shootTimer;
    void ShootPlayer();

public:
    CFloater() {};
    CFloater(int classId, int x, int y, int sectionId, int animsId);
    
    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void HandleTimerTick(LPTIMER sender);
};

typedef CFloater* LPFLOATER;
