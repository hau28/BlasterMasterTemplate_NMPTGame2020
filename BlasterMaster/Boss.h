#pragma once
#include "Enemy.h"
#include "BossArm.h"
#include "BossHand.h"

class CBoss : public CEnemy
{
    vector<CEnemy*> ArmLeft;
    vector<CEnemy*> ArmRight;
    
    void init_ObjectsArm();

    float limitTop, limitRight, limitLeft, limitBottom;
    float targeHandtLeftX, targetHandLeftY, targetHandRightX, targetHandRightY;
    void init_RandomTargetLeft();
    void init_RandomTargetRight();

    bool isRandomLocationArmLeft = true;
    bool isRandomLocationArmRight = true;

    void checkTargetLocation();

public:
    CBoss() {};
    CBoss(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};
    virtual void Render(float offsetX, float offsetY);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    virtual bool IsBlockableObject(LPGAMEOBJECT obj);
};
typedef CBoss* LPBOSS;


