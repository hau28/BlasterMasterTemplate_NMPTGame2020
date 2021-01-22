#pragma once
#include "Enemy.h"
#include "BossArm.h"
#include "BossHand.h"
#include "PlayScene.h"
#include "Timer.h"
#include "ObjectFlashingEffectPlayer.h"

const float BOSS_ARM_MOVING_MAX_SPEED = 0.1;
const float BOSS_ARM_MAX_PULL_FORCE = 0.02;
const int HEALTH_BOSS = 80;

class CBoss : public CEnemy, public ITimeTrackable
{
    vector<CEnemy*> ArmLeft;
    vector<CEnemy*> ArmRight;

    const int SHOT_PER_SHOOTING_PHASE = 4;
    const int DELAY_BETWEEN_SHOOTING_PHASES = 2800;
    const int DELAY_BETWEEN_SHOTS = 300;
    const int DELAY_IDLE_HAND_BOSS = 700;
    
    void init_ObjectsArm();

    float limitTop, limitRight, limitLeft, limitBottom;
    float targeHandtLeftX, targetHandLeftY, targetHandRightX, targetHandRightY;
    void init_RandomTargetLeft();
    void init_RandomTargetRight();

    void updateVectorHand();

    bool isRandomLocationArmLeft = true;
    bool isRandomLocationArmRight = true;

    LPTIMER shootPhaseTimer;
    LPTIMER singleShotTimer;
    LPTIMER delayIdleHandLeftBossTimer;
    LPTIMER delayIdleHandRightBossTimer;

    void checkTargetLocation();
    void updateVectorArmFollowBodyBoss(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

    float flagVx = 0, flagVy = 0;
    void ShootPlayer();

    bool isLoadedBossArm = false;

    void MoveArmLeft();
    void MoveArmRight();
    void RemoveArm();

    bool checkEquals(float x1, float y1, float x2, float y2);

    const int EXPLOSION_REMOVE_DURATION = 100;
    const int  COUNT_EXPLOSION = 50;
    LPTIMER explosionTimer;
    LPTIMER reloadTimer;

    int explodeCount = 0;
    bool flagexplode = false;
    bool flagDied = false;
    virtual void Explode();
    void DropItem();

public:
    CBoss() {};
    CBoss(int classId, int x, int y, int sectionId, int animsId);

    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void Render(float offsetX, float offsetY);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    virtual bool IsBlockableObject(LPGAMEOBJECT obj);

    virtual void HandleTimerTick(LPTIMER sender);
    void CalcBoundingBoxCenter(LPGAMEOBJECT obj, float& x, float& y);

    void GetPosition(float& x, float& y, float dx, float dy);

    void PlayFlashingEffect();
};
typedef CBoss* LPBOSS;


