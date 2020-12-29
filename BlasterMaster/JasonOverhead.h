#pragma once
#include "AnimatableObject.h"

// 2 25 21 31

const int JASONOVERHEAD_BOUNDBOX_WIDTH = 20;
const int JASONOVERHEAD_BOUNDBOX_HEIGHT = 9;
const int JASONOVERHEAD_BOUNDBOX_OFFSETX = 2;
const int JASONOVERHEAD_BOUNDBOX_OFFSETY = 25;

const int JASON_OVERHEAD_FACEDIR_UP = 0;
const int JASON_OVERHEAD_FACEDIR_DOWN = 1;
const int JASON_OVERHEAD_FACEDIR_LEFT = 2;
const int JASON_OVERHEAD_FACEDIR_RIGHT = 3;

class CJasonOverhead : public CAnimatableObject
{
private:
    const float JASON_OVERHEAD_MOVING_SPEED_X = 0.006f;
    const float JASON_OVERHEAD_MOVING_SPEED_Y = 0.006f;

    CJasonOverhead();
    void Init();

    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);

    int MovingDirX = 0;
    int MovingDirY = 0;

    bool IsMoving();

    int FaceDir = 1;

    void UpdateState();

    static CJasonOverhead* __instance;

public:
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    void SnapToPortalMiddle(LPGAMEOBJECT portal, bool snapX);

    static CJasonOverhead* GetInstance();
    static CJasonOverhead* InitInstance(int x, int y, int sectionId);
};

