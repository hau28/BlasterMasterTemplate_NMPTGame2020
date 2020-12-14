#pragma once
#include "Bullet.h"

const float BULLET_SOPHIA_SPEED = 0.2f;

class CBullet_Sophia : public CBullet
{
    // 0 10 11 13
    const int BULLET_SOPHIA_BOUNDBOX_WIDTH = 12;
    const int BULLET_SOPHIA_BOUNDBOX_HEIGHT = 4;
    const int BULLET_SOPHIA_BOUNDBOX_OFFSETX = 0;
    const int BULLET_SOPHIA_BOUNDBOX_OFFSETY = 9;

    const int BULLET_SOPHIA_SPRITE_WIDTH = 24;
    const int BULLET_SOPHIA_SPRITE_HEIGHT = 24;

    const int BULLET_SOPHIA_DAMAGE = 6;

public:
    CBullet_Sophia() {};

    CBullet_Sophia(int x, int y, int sectionId, int dirX, int dirY);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void CalcExplosionCenterPos(float& explosionX, float& explosionY);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CBullet_Sophia* LPBULLET_SOPHIA;
