#pragma once
#include "Enemy.h"
#include "Timer.h"


const int TELEPORTER_BOUNDBOX_WIDTH = 15;
const int TELEPORTER_BOUNDBOX_HEIGHT = 15;
const int TELEPORTER_BOUNDBOX_OFFSETX = 8;
const int TELEPORTER_BOUNDBOX_OFFSETY = 0;

const float TELEPORTER_FLY_SPEED = 0.06f;
const float TELEPORTER_BULLET_SPEED = 0.08f;

const int TELEPORTER_HEALTHPOINT = 3;


class CTeleporter : public CEnemy, public ITimeTrackable
{
private:

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
    CTeleporter() {};
    CTeleporter(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void HandleTimerTick(LPTIMER sender);
};

typedef CTeleporter* LPTELEPORTER;
