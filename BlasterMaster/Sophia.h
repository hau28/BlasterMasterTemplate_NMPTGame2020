#pragma once
#include "AnimatableObject.h"

// 8 23 31 39

const int SOPHIA_BOUNDBOX_WIDTH = 21;
const int SOPHIA_BOUNDBOX_HEIGHT = 16;
const int SOPHIA_BOUNDBOX_OFFSETX = 9;
const int SOPHIA_BOUNDBOX_OFFSETY = 23;
const float SOPHIA_MAX_SPEED = 0.1;

const float SOPHIA_GRAVITY = 0.007f;
const float SOPHIA_MAX_FALL_SPEED = 0.13f;
const float SOPHIA_JUMP_FORCE = 0.25f;

class CSophia :
    public CAnimatableObject
{
private:
    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);

    bool flagOnAir;
    bool flagDead=0;
    bool flagStop = false;
    bool stopLeft = false;
    bool isLeft = false;

	CSophia() {};
	CSophia(int classId, int x, int y, int animsId);

    static CSophia* __instance;
public:
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjs);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    static CSophia* GetInstance();
    static CSophia* InitInstance(int classId, int x, int y, int animsId);
};

typedef CSophia* LPSOPHIA;
