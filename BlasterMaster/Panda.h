#pragma once
#include "Enemy.h"

// bound 8 12 23 18

const int PANDA_BOUNDBOX_WIDTH = 15;
const int PANDA_BOUNDBOX_HEIGHT = 6;
const int PANDA_BOUNDBOX_OFFSETX = 8;
const int PANDA_BOUNDBOX_OFFSETY = 12;

const float PANDA_GRAVITY = 0.001f;
const float PANDA_MAX_FALL_SPEED = 0.03f;
const float PANDA_MOVE_SPEED = 0.05f;

class CPanda :
    public CEnemy
{
private:
    bool flagDead = false;
    bool flagOnAir = true;

public:
    CPanda() {};
    CPanda(int classId, int x, int y, int animsId);
    
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};

typedef CPanda* LPPANDA;
