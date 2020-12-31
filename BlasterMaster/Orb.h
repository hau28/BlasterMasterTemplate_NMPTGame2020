#pragma once
#pragma once
#include "Enemy.h"

const int ORB_BOUNDBOX_WIDTH = 14;
const int ORB_BOUNDBOX_HEIGHT = 14;
const int ORB_BOUNDBOX_OFFSETX = 2;
const int ORB_BOUNDBOX_OFFSETY = 2;

const float ORB_MOVE_SPEED = 0.05f;
const float ORB_CHASE_SPEED = 0.09f;
const DWORD VERTICAL_PERIOD = 5000;

class COrb : public CEnemy
{
private:
    const int ORB_HEALTHPOINT = 6;

    void UpdateState();
    DWORD lastTimeVertical;
    float yStartFalling;
    bool suicide;
    int flagTurn;
public:
    COrb() {};
    COrb(int classId, int x, int y, int sectionId, int initLeft, int animsId, bool behaviorId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef COrb* LPORB;
