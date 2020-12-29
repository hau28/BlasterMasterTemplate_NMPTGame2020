#pragma once
#include "AnimatableObject.h"
class CExplosion : public CAnimatableObject
{
private:
    const int EXPLOSION_BOUNDBOX_WIDTH = 24;
    const int EXPLOSION_BOUNDBOX_HEIGHT = 24;
    const int EXPLOSION_BOUNDBOX_OFFSETX = 0;
    const int EXPLOSION_BOUNDBOX_OFFSETY = 0;

public:
    CExplosion() {};
    CExplosion(int classId, int x, int y, int sectionId, int animsId);
    CExplosion(int classId, int x = 0, int y = 0, int sectionId = 0);


    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void Render(float offsetX, float offsetY);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    virtual void ResolveInteractions(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CExplosion* LPEXPLOSION;

