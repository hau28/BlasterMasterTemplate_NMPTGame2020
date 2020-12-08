#pragma once
#include "AnimatableObject.h"
class CBullet : public CAnimatableObject
{
protected:
    CBullet() {};
    CBullet(int x, int y, int sectionId, int animsId, bool isFriendly);

public:
    bool isFriendly = false;

    virtual void UpdateVelocity(DWORD dt) = 0;
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) = 0;
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) = 0;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
};

typedef CBullet* LPBULLET;
