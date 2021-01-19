#pragma once
#include "Enemy.h"
#include "Timer.h"


const int EYEBALL_BOUNDBOX_WIDTH = 14;
const int EYEBALL_BOUNDBOX_HEIGHT = 4;
const int EYEBALL_BOUNDBOX_OFFSETX = 1;
const int EYEBALL_BOUNDBOX_OFFSETY = 30;

const float EYEBALL_FLY_SPEED = 0.15f;
const float EYEBALL_STOP_SPEED = 0.004f;
const float EYEBALL_BULLET_SPEED = 0.08f;

const int EYEBALL_HEALTHPOINT = 1;



class CEyeball : public CEnemy, public ITimeTrackable
{
private:
    const int DELAY_BETWEEN_MOVE = 2500;
    bool flagMove;
    bool flagRight;
    bool flagDown;
    int decision;
    float stopSpeed;
    void UpdateState();

    LPTIMER moveTimer;
    void ShootPlayer();

public:
    CEyeball() {};
    CEyeball(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt) {};
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};
    void approach();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void HandleTimerTick(LPTIMER sender);
};

typedef CEyeball* LPEYEBALL;
