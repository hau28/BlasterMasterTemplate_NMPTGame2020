#pragma once
#include "Enemy.h"

class CSkull : public CEnemy
{

    const int SKULL_BOUNDBOX_WIDTH = 18;
    const int SKULL_BOUNDBOX_HEIGHT =21 ;
    const int SKULL_BOUNDBOX_OFFSETX = 7;
    const int SKULL_BOUNDBOX_OFFSETY = 7;

    const float SKULL_MOVE_SPEED = 0.2f;
    const int BOX = 5;

private: 
    void UpdateState();
    bool flagshootbullet = false;
    bool flagtouchwall = false;
    int firstshoot = 0;
    float oldVX, oldVY;

public:
    CSkull() {};
    CSkull(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CSkull* LPSKULL;
