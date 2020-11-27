#pragma once
#include "AnimatableObject.h"
#include <queue>
#include "WalkInPortalEvent.h"

// 8 8 15 23

const int JASONSIDEVIEW_BOUNDBOX_WIDTH = 7;
const int JASONSIDEVIEW_BOUNDBOX_HEIGHT = 15;
const int JASONSIDEVIEW_BOUNDBOX_OFFSETX = 8;
const int JASONSIDEVIEW_BOUNDBOX_OFFSETY = 8;

const float JASONSIDEVIEW_GRAVITY = 0.0005f;

class CJasonSideview : public  CAnimatableObject
{
private:

    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);

    bool flagOnAir;

    CJasonSideview();
    CJasonSideview(int classId, int x, int y, int animsId);

    static CJasonSideview* __instance;

public: 
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    static CJasonSideview* GetInstance();
    static CJasonSideview* InitInstance(int x, int y, int sectionId);

};

typedef CJasonSideview* LPJASONSIDEVIEW;