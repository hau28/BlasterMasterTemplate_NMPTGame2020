#pragma once
#include "AnimatableObject.h"
class CExplosion : public CAnimatableObject
{
protected:
    CExplosion() {};
    CExplosion(int x, int y, int sectionId, int animsId);

public:
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void Render(float offsetX, float offsetY);
    virtual void ResolveInteractions(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
};
typedef CExplosion* LPEXPLOSION;

