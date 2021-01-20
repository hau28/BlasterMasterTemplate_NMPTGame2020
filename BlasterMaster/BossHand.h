#pragma once
#include "Enemy.h"

const int BOSSHAND_BOUNDBOX_WIDTH = 10;
const int BOSSHAND_BOUNDBOX_HEIGHT = 10;
const int BOSSHAND_BOUNDBOX_OFFSETX = 3;
const int BOSSHAND_BOUNDBOX_OFFSETY = 3;

const float BOSSHAND_GRAVITY = 0.015f;
const float BOSSHAND_MAX_FALL_SPEED = 0.14f;
const float BOSSHAND_MOVE_SPEED = 0.06f;

class CBossHand : public CEnemy
{
public:
    CBossHand() {};
    CBossHand(int classId, int x, int y, int sectionId, int animsId, int state);
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    virtual bool IsBlockableObject(LPGAMEOBJECT obj);
};
typedef CBossHand* LPBOSSHAND;
