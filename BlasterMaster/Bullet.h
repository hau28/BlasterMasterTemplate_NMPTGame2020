#pragma once
#include "AnimatableObject.h"
#include "Explosion.h"

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

    virtual void CalcExplosionCenterPos(float& explosionX, float& explosionY);

    /// <summary>
    /// <para> explosion: only passed in to know the type of explosion to create. </para>
    /// <para> this function would set the exploding position itself </para>
    /// </summary>
    virtual void Explode(int explosionClassId);
};

typedef CBullet* LPBULLET;
