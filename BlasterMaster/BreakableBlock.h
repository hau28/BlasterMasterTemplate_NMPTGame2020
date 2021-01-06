#pragma once
#include "AnimatableObject.h"
class CBreakableBlock : public CAnimatableObject
{
private:
    const int BREAKABLE_BOUNDBOX_OFFSETX = 0;
    const int BREAKABLE_BOUNDBOX_OFFSETY = 0;
    const int BREAKABLE_BOUNDBOX_WIDTH = 16;
    const int BREAKABLE_BOUNDBOX_HEIGHT = 16;

    bool isDestroyed = false;
    bool requireCrusherBeam = false;

public:
    CBreakableBlock() {};
    CBreakableBlock(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void Explode();

    virtual bool IsDestroyed() { return isDestroyed; };
};

