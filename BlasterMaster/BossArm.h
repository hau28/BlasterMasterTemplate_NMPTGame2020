#pragma once
#include "Enemy.h"

const int BOSSARM_BOUNDBOX_WIDTH = 10;
const int BOSSARM_BOUNDBOX_HEIGHT = 10;
const int BOSSARM_BOUNDBOX_OFFSETX = 3;
const int BOSSARM_BOUNDBOX_OFFSETY = 3;

const float BOSSARM_GRAVITY = 0.015f;
const float BOSSARM_MAX_FALL_SPEED = 0.14f;
const float BOSSARM_MOVE_SPEED = 0.06f;

class CBossArm : public CEnemy
{

public:
    CBossArm() {};
    CBossArm(int classId, int x, int y, int sectionId, int animsId, int state);
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual bool IsBlockableObject(LPGAMEOBJECT obj);
};

typedef CBossArm* LPBOSSARM;
