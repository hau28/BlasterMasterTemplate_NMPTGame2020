#pragma once
#include "AnimatableObject.h"
#include <queue>
#include "WalkInPortalEvent.h"
#include "Timer.h"
#include "ObjectFlashingEffectPlayer.h"

// 8 8 15 23

const int JASONSIDEVIEW_BOUNDBOX_WIDTH = 7;
const int JASONSIDEVIEW_BOUNDBOX_HEIGHT = 15;
const int JASONSIDEVIEW_BOUNDBOX_OFFSETX = 8;
const int JASONSIDEVIEW_BOUNDBOX_OFFSETY = 8;

const int JASONSIDEVIEW_CRAWL_BOUNDBOX_HEIGHT = 8;

const float JASONSIDEVIEW_GRAVITY = 0.00065f;
const float JASONSIDEVIEW_VX = 0.06f;
const float JASONSIDEVIEW_AX = -0.0004f;
//const float JASONSIDEVIEW_MAX_FALL_SPEED = 0.07f;
const float JASONSIDEVIEW_JUMP_SPEED_Y = 0.225f;

const float JASONSIDEVIEW_CLIMB_SPEED = 0.06f;

class CJasonSideview : public  CAnimatableObject, public ITimeTrackable
{
private:
    const int DYING_EFFECT_DURATION = 1700;

    // CuteTN: Flashing effect on damage
    const int JASONSIDEVIEW_VULNERABLE_EFFECT_FLASHING_DURATION = 30;
    CObjectFlashingEffectPlayer* vulnerableFlashingEffect = nullptr;
    void PlayVulnerableFlasingEffect();
    void HandleOnDamage();
    void CheckDistance(float & yStartFalling, float& yEndFalling);

    // CuteTN: knock back on damage
    const float JASONSIDEVIEW_KNOCKEDBACK_VX = 0.07;
    const float JASONSIDEVIEW_KNOCKEDBACK_VY = 0.07;
    const float JASONSIDEVIEW_JUMP_KNOCKEDBACK_VY = 0.001;

    // CuteTN Note: we cannot handle knocked back right in handle overlap, it can't detect collision if you set there
    bool flagKnockedBack = false;

    // CuteTN Note: please call this method in UpdateVelocity
    void BeKnockedBack();

    void HandleKeys(DWORD dt);
    void HandleKeyUp(DWORD dt, int keyCode);
    void HandleKeyDown(DWORD dt, int keyCode);
    void HandleKeysHold(DWORD dt);

    CJasonSideview();
    CJasonSideview(int classId, int x, int y, int animsId);

    static CJasonSideview* __instance;

    bool flagOnAir = false;
    bool flagTurnRight = true;
    bool flagCrawl = false;
    bool flagJumpWalk;
    bool flagWalk;
    bool flagCanClimb = false;
    bool flagClimb;
    bool flagClimbOver;
    bool flagSwim = false;

    float ladderL = 48, ladderT = 158, ladderR = 64, ladderB = 350, jason_l, jason_t, jason_r, jason_b;

    bool flagInvulnerable;

    LPTIMER invulnerableTimer = nullptr;
    LPTIMER dyingEffectTimer = nullptr;

    DWORD timeJumpIn ;
    //LEFT 2
    bool flagFinishAnimationJasonDead = false;

    float yStartFalling, yEndFalling;

    // CuteTN: switching scene with scene portal
    LPGAMEOBJECT overlappingScenePortal = nullptr;

    // CuteTN: reload and limit bullets
    const int JASONSIDEVIEW_BULLET_RELOAD_DURATION = 100;
    bool flagBulletReloaded;
    LPTIMER bulletReloadTimer;

    const int JASONSIDEVIEW_MAX_BULLETS_ON_CAMERA = 2;
    int numberOfJasonSideviewBullets;
    void CountJasonSideviewBullets(vector<LPGAMEOBJECT>* coObjs);

    void Shoot();

public:     
    virtual void UpdateVelocity(DWORD dt);
    virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
    virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
    virtual void Render(float offsetX, float offsetY);
    static CJasonSideview* GetInstance();
    static CJasonSideview* InitInstance(int x, int y, int sectionId);
    //SANH-BOUNDINGBOX JASON
    float camBoxLeft, camBoxRight, camBoxTop, camBoxBottom;


    void init_camBox();
    void resetState();
    void Init();
    
    virtual void HandleTimerTick(LPTIMER sender);
    
    void keyUptoFixState() { HandleKeys(15); HandleKeyDown(15, DIK_UP); }

    void Start_invulnerableTimer();

    ~CJasonSideview();
};

typedef CJasonSideview* LPJASONSIDEVIEW;