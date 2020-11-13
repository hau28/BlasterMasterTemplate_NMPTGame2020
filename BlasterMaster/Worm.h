#pragma once
#include "Enemy.h"

// 8 12 23 18

const int WORM_BOUNDBOX_WIDTH = 15;
const int WORM_BOUNDBOX_HEIGHT = 6;
const int WORM_BOUNDBOX_OFFSETX = 8;
const int WORM_BOUNDBOX_OFFSETY = 12;

const float WORM_GRAVITY = 0.001f;
const float WORM_MAX_FALL_SPEED = 0.01f;
const float WORM_MOVE_SPEED = 0.01f;

class CWorm :
    public CEnemy
{
private:
    bool flagDead = false;
    bool flagOnAir = true;

public:
    CWorm() {};
    CWorm(int classId, int x, int y, int animsId);
    
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};

typedef CWorm* LPWORM;
