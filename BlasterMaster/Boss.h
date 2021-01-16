#pragma once
#include "Enemy.h"
#include "BossArm.h"
#include "BossHand.h"

class CBoss : public CEnemy
{
    vector<CBossArm*> ArmLeft;
    vector<CBossArm*> ArmRight;
    CBossHand* HandLeft;
    CBossHand* HandRight;
    
    void init_ObjectsArm();
public:
    CBoss() {};
    CBoss(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};
    virtual void Render(float offsetX, float offsetY);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};
typedef CBoss* LPBOSS;


