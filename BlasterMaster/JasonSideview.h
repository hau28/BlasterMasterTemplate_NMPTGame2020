#pragma once
#include "AnimatableObject.h"
#include <queue>
#include "WalkInPortalEvent.h"

// 8 8 15 23

const int JASONSIDEVIEW_BOUNDBOX_WIDTH = 7;
const int JASONSIDEVIEW_BOUNDBOX_HEIGHT = 15;
const int JASONSIDEVIEW_BOUNDBOX_OFFSETX = 8;
const int JASONSIDEVIEW_BOUNDBOX_OFFSETY = 8;

const float JASONSIDEVIEW_GRAVITY = 0.00065f;
const float JASONSIDEVIEW_VX = 0.06;
//const float JASONSIDEVIEW_MAX_FALL_SPEED = 0.07f;
const float JASONSIDEVIEW_JUMP_SPEED_Y = 0.225f;

class CJasonSideview : public  CAnimatableObject
{
private:

    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);

    CJasonSideview();
    CJasonSideview(int classId, int x, int y, int animsId);

    static CJasonSideview* __instance;

    bool flagOnAir = false;
    bool Jason_turnRight = true;
    bool flag_keydown = false;
    //bool Jasonn_crawl = false;

    //vector<int> Jason_idleLeftStates = { SOPHIA_STATE_IDLE_LEFT,SOPHIA_STATE_IDLE1_LEFT,SOPHIA_STATE_IDLE2_LEFT,SOPHIA_STATE_IDLE3_LEFT };


public:     
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    static CJasonSideview* GetInstance();
    static CJasonSideview* InitInstance(int x, int y, int sectionId);
};

typedef CJasonSideview* LPJASONSIDEVIEW;