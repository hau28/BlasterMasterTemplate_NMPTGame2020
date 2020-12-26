#pragma once
#include "AnimatableObject.h"
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

