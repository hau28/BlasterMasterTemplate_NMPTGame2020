#pragma once
#pragma once
#include "Enemy.h"

const int JUMPER_BOUNDBOX_WIDTH = 13;
const int JUMPER_BOUNDBOX_HEIGHT = 22;
const int JUMPER_BOUNDBOX_OFFSETX = 1;
const int JUMPER_BOUNDBOX_OFFSETY = 2;

const float JUMPER_GRAVITY = 0.02f;
const float JUMPER_MAX_SPEED = 0.6;
const float JUMPER_MOVE_SPEED = 0.04f;
const float JUMPER_JUMP_FORCE = 0.3f;

const DWORD JUMP_PERIOD = 4000;

class CJumper : public CEnemy
{
private:
    const int JUMPER_HEALTHPOINT = 6;

    void UpdateState();
    bool flagOnAir;
    int flagOnEdge;
    DWORD lastTimeJump;
    float ground;
    int numJump;
public:
    CJumper() {};
    CJumper(int classId, int x, int y, int sectionId, int initLeft, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};

typedef CJumper* LPJUMPER;
