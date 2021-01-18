#pragma once
#include "Enemy.h"
#include "BossArm.h"
#include "BossHand.h"
#include "Timer.h"

class CBoss : public CEnemy, public ITimeTrackable
{
    vector<CEnemy*> ArmLeft;
    vector<CEnemy*> ArmRight;

    const int SHOT_PER_SHOOTING_PHASE = 4;
    const int DELAY_BETWEEN_SHOOTING_PHASES = 2800;
    const int DELAY_BETWEEN_SHOTS = 400;
    
    void init_ObjectsArm();

    float limitTop, limitRight, limitLeft, limitBottom;
    float targeHandtLeftX, targetHandLeftY, targetHandRightX, targetHandRightY;
    void init_RandomTargetLeft();
    void init_RandomTargetRight();

    bool isRandomLocationArmLeft = true;
    bool isRandomLocationArmRight = true;

    LPTIMER shootPhaseTimer;
    LPTIMER singleShotTimer;

    void checkTargetLocation();
    void ShootPlayer();

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

    virtual void HandleTimerTick(LPTIMER sender);
    void CalcBoundingBoxCenter(LPGAMEOBJECT obj, float& x, float& y);
};
typedef CBoss* LPBOSS;


