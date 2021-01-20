#pragma once
#include "Enemy.h"
#include "Timer.h"


const int TELEPORTER_BOUNDBOX_WIDTH = 22;
const int TELEPORTER_BOUNDBOX_HEIGHT = 8;
const int TELEPORTER_BOUNDBOX_OFFSETX = 5;
const int TELEPORTER_BOUNDBOX_OFFSETY = 24;

const float TELEPORTER_FLY_SPEED = 0.06f;
const float TELEPORTER_BULLET_SPEED = 0.08f;

const int TELEPORTER_HEALTHPOINT = 2;

class CTeleporter : public CEnemy, public ITimeTrackable
{
private:
    const int IDLE_DURATION = 750;
    const int TELEPORT_DURATION = 18;
    const int TELEPORT_TIMES = 18;
    int flagDoneTeleport;
    void UpdateState();
    LPTIMER idleTimer;
    LPTIMER teleportTimer;
    LPTIMER switchTimer;
    void ShootPlayer();
    bool flagStop;
    void calcTele();
    float vxTele, vyTele;
public:
    CTeleporter() {};
    CTeleporter(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void HandleTimerTick(LPTIMER sender);
};

typedef CTeleporter* LPTELEPORTER;
