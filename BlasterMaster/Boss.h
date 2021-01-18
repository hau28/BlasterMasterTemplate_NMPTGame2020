#pragma once
#include "Enemy.h"
#include "BossArm.h"
#include "BossHand.h"
#include "PlayScene.h"

const float BOSS_ARM_MOVING_MAX_SPEED = 0.1;
const float BOSS_ARM_MAX_PULL_FORCE = 0.02;
//const float BOSS_ARM_

class CBoss : public CEnemy
{
    vector<CEnemy*> ArmLeft;
    vector<CEnemy*> ArmRight;
    
    void init_ObjectsArm();

    float limitTop, limitRight, limitLeft, limitBottom;
    float targeHandtLeftX, targetHandLeftY, targetHandRightX, targetHandRightY;
    void init_RandomTargetLeft();
    void init_RandomTargetRight();

    void updateVectorHand();

    bool isRandomLocationArmLeft = true;
    bool isRandomLocationArmRight = true;

    void checkTargetLocation();
    void updateVectorArmFollowBodyBoss(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    float flagVx = 0, flagVy = 0;

    bool isLoadedBossArm = false;

    void MoveArmLeft();
    void MoveArmRight();
public:
    CBoss() {};
    CBoss(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) {};
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    virtual bool IsBlockableObject(LPGAMEOBJECT obj);
};
typedef CBoss* LPBOSS;


