#pragma once
#include "AnimatableObject.h"
#include "Timer.h"
#include "ObjectFlashingEffectPlayer.h"
#include <queue>

// 2 25 21 31

const int JASONOVERHEAD_BOUNDBOX_WIDTH = 14;
const int JASONOVERHEAD_BOUNDBOX_HEIGHT = 5;
const int JASONOVERHEAD_BOUNDBOX_OFFSETX = 5;
const int JASONOVERHEAD_BOUNDBOX_OFFSETY = 27;

const int JASON_OVERHEAD_FACEDIR_UP = 0;
const int JASON_OVERHEAD_FACEDIR_DOWN = 1;
const int JASON_OVERHEAD_FACEDIR_LEFT = 2;
const int JASON_OVERHEAD_FACEDIR_RIGHT = 3;

class CJasonOverhead : public CAnimatableObject, public ITimeTrackable
{
private:
    const float JASON_OVERHEAD_MOVING_SPEED_X = 0.006f;
    const float JASON_OVERHEAD_MOVING_SPEED_Y = 0.006f;
    const int DYING_EFFECT_DURATION = 2000;


    const float GRENADE_DISTANCE = 2.5 * 16;
    const float GRENADE_DISTANCE_FAR = 4 * 16;

    CJasonOverhead();
    void Init();

    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);

    int MovingDirX = 0;
    int MovingDirY = 0;

    bool IsMoving();
    float distance;

    int FaceDir = 1;

    bool flagInvulnerable;

    bool flagBulletReloaded;
    LPTIMER bulletReloadTimer;

    const int JASONOVERHEAD_BULLET_RELOAD_DURATION = 100;
    const int JASONOVERHEAD_GRENADE_RELOAD_DURATION = 50;
    //const int JASONOVERHEAD_BULLETLEVEL45_RELOAD_DURATION = 300;

    bool flaggrenadeReloaded;
    LPTIMER grenadeReloadTimer;

    LPTIMER invulnerableTimer = nullptr;
    LPTIMER dyingEffectTimer = nullptr;
    bool flagDead = false;

    void UpdateState();
    void GetShootPosition(float& x, float& y, float dx, float dy);
    void GetDropBombPosition(float& x, float& y, float dx, float dy);
    int max_bullets_on_cam = 2;
    int numberOfJasonOverheadBullets;
    void CountJasonOverheadBullets(vector<LPGAMEOBJECT>* coObjs);
    void Shoot();
    void DropBomb();
    void checkDxDy(float& dx, float& dy);

    static CJasonOverhead* __instance;

    int gunlevel = 4;

    void HandleOnDamage();

    // CuteTN: Flashing effect on damage
    const int JASONOVERHEAD_VULNERABLE_EFFECT_FLASHING_DURATION = 30;
    CObjectFlashingEffectPlayer* vulnerableFlashingEffect = nullptr;
    void PlayVulnerableFlasingEffect();

public:
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    void SnapToPortalMiddle(LPGAMEOBJECT portal, bool snapX);

    virtual void HandleTimerTick(LPTIMER sender);

    static CJasonOverhead* GetInstance();
    static CJasonOverhead* InitInstance(int x, int y, int sectionId);


};

