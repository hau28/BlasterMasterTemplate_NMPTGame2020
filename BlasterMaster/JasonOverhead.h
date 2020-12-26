#pragma once
#include "AnimatableObject.h"

// 2 25 21 31

const int JASONOVERHEAD_BOUNDBOX_WIDTH = 20;
const int JASONOVERHEAD_BOUNDBOX_HEIGHT = 7;
const int JASONOVERHEAD_BOUNDBOX_OFFSETX = 2;
const int JASONOVERHEAD_BOUNDBOX_OFFSETY = 25;

class CJasonOverhead : public CAnimatableObject
{
private:
    CJasonOverhead();
    void Init();

    static CJasonOverhead* __instance;

public:
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    // virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    static CJasonOverhead* GetInstance();
    static CJasonOverhead* InitInstance(int x, int y, int sectionId);
};

