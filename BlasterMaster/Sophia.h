#pragma once
#include "AnimatableObject.h"

// 8 23 31 39

const int SOPHIA_BOUNDBOX_WIDTH = 21;
const int SOPHIA_BOUNDBOX_HEIGHT = 16;
const int SOPHIA_BOUNDBOX_OFFSETX = 9;
const int SOPHIA_BOUNDBOX_OFFSETY = 23;
const float SOPHIA_MAX_SPEED = 0.1;

const float SOPHIA_GRAVITY = 0.01f;

class CSophia :
    public CAnimatableObject
{
private:
    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);
    void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    bool flagOnAir;
    bool flagDead=0;
    bool flagStop = false;
    bool stopLeft = false;
    bool isLeft = false;
    DWORD lastTimeMoveWheel;
    int wheel = 0;
public:
	CSophia() {};
	CSophia(int classId, int x, int y, int animsId);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjs);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CSophia* LPSophia;
