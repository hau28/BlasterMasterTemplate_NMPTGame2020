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
    static const float BULLET_OVERHEAD_OFFSET_FROM_SHADOW;

    // isFriendly = true when this bullet cannot damage the player.
    bool isFriendly = false;
    int damage = 0;
    void SetBoundingBoxInfo(int classId);

    virtual void UpdateVelocity(DWORD dt) = 0;
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) = 0;
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) = 0;

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual bool IsBlockableObject(LPGAMEOBJECT obj);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void CalcExplosionCenterPos(float& explosionX, float& explosionY);

    /// <summary>
    /// <para> Destroy this bullet and create explosion effect </para>
    /// <para> this function would set the exploding position itself </para>
    /// </summary>
    virtual void Explode(int explosionClassId);

    virtual void ExplodeGrenade(DWORD timetolive);

    virtual void Render(float offsetX, float offsetY);
};

typedef CBullet* LPBULLET;
