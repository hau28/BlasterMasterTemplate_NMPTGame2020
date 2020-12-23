#pragma once
#include "AnimatableObject.h"
#include <queue>
#include "WalkInPortalEvent.h"
#include <math.h>
#include "Timer.h"
#include "ObjectFlashingEffectPlayer.h"

// 8 23 31 39

const int SOPHIA_BOUNDBOX_WIDTH = 21;
const int SOPHIA_BOUNDBOX_HEIGHT = 15;
const int SOPHIA_BOUNDBOX_OFFSETX = 10;
const int SOPHIA_BOUNDBOX_OFFSETY = 23;

const int SOPHIA_SPRITE_WIDTH = 40;
const int SOPHIA_SPRITE_HEIGHT = 40;

const float SOPHIA_MAX_SPEED = 0.1;
const float SOPHIA_ENGINE = 0.0005;
const float FRICTION = 0.00025;

const float SOPHIA_GRAVITY = 0.0005f;
const float SOPHIA_MAX_FALL_SPEED = 0.175f;
const float SOPHIA_JUMP_FORCE = 0.26f;

const int INVULNERABLE_DURATION = 450;

/// <summary>
/// SOPHIA is also animatable, however, it has a completely different kind of animation system
/// so we have to somehow override the render method
/// </summary>
class CSophia : public CAnimatableObject , public ITimeTrackable
{
private:
    const int DYING_EFFECT_DURATION = 1700;

    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);
    DWORD lastTimeupdateWheel;
    DWORD lastTimeupdateDirection;
    DWORD lastTimeupdateGun;
    DWORD lastTimeUpdateBody;
    int portaling;
    virtual void updateWheel();
    virtual void updateDirection();
    virtual void updateBody();
    virtual void updateGun();

    const int SOPHIA_VULNERABLE_EFFECT_FLASHING_DURATION = 30;
    CObjectFlashingEffectPlayer* vulnerableFlashingEffect = nullptr;
    void PlayVulnerableFlasingEffect();
    void HandleOnDamage();

    int directionState, gunState, bodyState, wheelState;

    bool flagOnAir;
    bool flagDead = 0;
    float ground;
    bool turnRight;
    bool flagInvulnerable;

    LPTIMER invulnerableTimer;
    LPTIMER dyingEffectTimer = nullptr;

    CSophia(){};
    CSophia(int classId, int x, int y);
    void Init(int classId, int x, int y);

    static CSophia *__instance;
    bool flag_JasonJumpOut;

    // CuteTN: shoot
    void GetShootPosition(float& x, float &y);
    void GetGunDirection(float& dirX, float& dirY);


public:
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjs);
    virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
    virtual void Render(float offsetX, float offsetY);

    void jasonJumpIn();
    //0: trai, 1: dang xoay trai, 2: dang xoay phai, 3: phai
    void setGunState(int state);
    //0: trai, 1: dang xoay trai, 2: dang xoay phai, 3: phai

    float camBoxLeft, camBoxRight,camBoxTop,camBoxBottom;
    void roundPosition() { x = round(x); y = round(y); }
    void roundPositionX() { x = round(x); }
    
    void init_camBox();
    void init_camBox_FollowCamera();
    static CSophia *GetInstance();
    static CSophia *InitInstance(int classId, int x, int y, int sectionId);
    virtual void HandleTimerTick(LPTIMER sender);

    ~CSophia();
};

typedef CSophia *LPSOPHIA;
