#pragma once
#include "AnimatableObject.h"

// 8 12 23 18

const int WORM_BOUNDBOX_WIDTH = 15;
const int WORM_BOUNDBOX_HEIGHT = 6;
const int WORM_BOUNDBOX_OFFSETX = 8;
const int WORM_BOUNDBOX_OFFSETY = 12;

const float WORM_GRAVITY = 0.01f;

class CWorm :
    public CAnimatableObject
{
private:
    void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    bool flagDead = false;
    bool flagOnAir = true;

public:
    CWorm() {};
    CWorm(int classId, int x, int y, int animsId);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};

typedef CWorm* LPWORM;
