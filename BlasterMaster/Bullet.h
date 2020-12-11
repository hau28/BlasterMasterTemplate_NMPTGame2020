#pragma once
#include "AnimatableObject.h"
#include "Explosion.h"

class CBullet : public CAnimatableObject
{
private:
    float boundingBoxOffsetX = 0;
    float boundingBoxOffsetY = 0;
    float boundingBoxWidth = 0;
    float boundingBoxHeight = 0;

protected:
    CBullet() {};
    CBullet(int classId, int x, int y, int sectionId, bool isFriendly);

public:
    // isFriendly = true when this bullet cannot damage the player.
    bool isFriendly = false;
    int ChooseAnimationsId(int classId);
    void SetBoundingBoxInfo(int classId);

    virtual void UpdateVelocity(DWORD dt) = 0;
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) = 0;
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) = 0;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void CalcExplosionCenterPos(float& explosionX, float& explosionY);

    /// <summary>
    /// <para> Destroy this bullet and create explosion effect </para>
    /// <para> this function would set the exploding position itself </para>
    /// </summary>
    virtual void Explode(int explosionClassId);
};

typedef CBullet* LPBULLET;
