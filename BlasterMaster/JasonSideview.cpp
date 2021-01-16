#include "JasonSideview.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include "Sophia.h"
#include "PlayScene.h"
#include "GameGlobal.h"
#include <algorithm>
#include <assert.h>
#include "JasonJumpInEvent.h"
#include "Bullet_JasonSideview.h"
#include "SwitchSceneEvent.h"
#include "Sound.h"

CJasonSideview* CJasonSideview::__instance = nullptr;

CJasonSideview::CJasonSideview()
{
    Init();
}

CJasonSideview::CJasonSideview(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, -1, animsId)
{
    SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    init_camBox();

    vy = 0;

    invulnerableTimer = new CTimer(this, INVULNERABLE_DURATION, 1);
    invulnerableTimer->Stop();
    flagInvulnerable = false;

};

void CJasonSideview::init_camBox()
{
    //Init CamBox Jason must not to lag camera (original position camera)   
    float cameraX, cameraY;
    CGame::GetInstance()->GetCamPos(cameraX, cameraY);
    float centerPointX = cameraX + CGame::GetInstance()->GetScreenWidth() / 2;
    float centerPointY = cameraY + CGame::GetInstance()->GetScreenHeight() / 2;

    camBoxLeft = centerPointX - 16*2;
    camBoxRight  = camBoxLeft + 16* 4;
    camBoxBottom = centerPointY + 16*2;
    camBoxTop = camBoxBottom - 16 * 6;
    
    vx *= 3;
    vy = 0;
}

void CJasonSideview::resetState()
{
    if (flagTurnRight)
        SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    else
        SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);

    vy = 0; 
    vx = 0;
}

void CJasonSideview::Init()
{
    classId = CLASS_JASONSIDEVIEW;
    SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    // CuteTN Note: DIRTY AF
    LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(JASONSIDEVIEW_ANIMATIONS);
    animationHandlers = objAnims->GenerateAnimationHanlders();

    this->allowOverlapWithBlocks = true;

    invulnerableTimer = new CTimer(this, INVULNERABLE_DURATION, 1);
    invulnerableTimer->Stop();

    dyingEffectTimer = new CTimer(this, DYING_EFFECT_DURATION, 1);
    dyingEffectTimer->Stop();

    flagInvulnerable = false;
    flagCrawl = false;
    flagCanClimb = false;
    flagSwim = false;

    modifyR = modifyG = modifyB = 255;

    vulnerableFlashingEffect = new CObjectFlashingEffectPlayer(this, &flashingColors, JASONSIDEVIEW_VULNERABLE_EFFECT_FLASHING_DURATION);

    // CuteTN
    flagBulletReloaded = true;
    bulletReloadTimer = new CTimer(this, JASONSIDEVIEW_BULLET_RELOAD_DURATION, 1);
    bulletReloadTimer->Stop();
}

#pragma region key events handling

void CJasonSideview::PlayVulnerableFlasingEffect()
{
    if (vulnerableFlashingEffect)
        vulnerableFlashingEffect->Play();
}

void CJasonSideview::HandleOnDamage()
{
    Sound::getInstance()->play(JASON_GOT_HIT, false, 1);
	flagInvulnerable = true;
	invulnerableTimer->Start();
    PlayVulnerableFlasingEffect();
}

void CJasonSideview::CheckDistance(float& yStartFalling, float& yEndFalling)
{
    if (yEndFalling - yStartFalling < 40)
        yStartFalling = yEndFalling;

    if (40 <= (yEndFalling - yStartFalling) && 50>= (yEndFalling - yStartFalling))
    {
        vy -= JASONSIDEVIEW_KNOCKEDBACK_VY;
        CGameGlobal::GetInstance()->beingAttackedByLowFall();
        yStartFalling = yEndFalling;
        HandleOnDamage();
        flagKnockedBack = true;
    }
                
    if (yEndFalling - yStartFalling > 50)
    {
        vy -= JASONSIDEVIEW_KNOCKEDBACK_VY;
        CGameGlobal::GetInstance()->beingAttackedByDrop();
        HandleOnDamage();
        flagKnockedBack = true;
    }
}

void CJasonSideview::BeKnockedBack()
{
    if (!flagKnockedBack)
        return;

    if (flagCrawl)
    {
        int posX = rand()%10;

        flagCrawl = false;

        this->x -= posX;

        if (flagTurnRight)
            SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    }
    else
    {
        if (!flagOnAir)
            vy -= JASONSIDEVIEW_KNOCKEDBACK_VY;

        if (!flagClimb)
        {
            if (!flagOnAir)
            {
                if (flagTurnRight)
                    vx -= JASONSIDEVIEW_KNOCKEDBACK_VX;
                else
                    vx += JASONSIDEVIEW_KNOCKEDBACK_VX;
            }
        }
        else
        {
            flagClimb = false;
            vy -= JASONSIDEVIEW_KNOCKEDBACK_VY;

            if (flagTurnRight)
                vx -= JASONSIDEVIEW_KNOCKEDBACK_VX / 2;
            else
                vx += JASONSIDEVIEW_KNOCKEDBACK_VX / 2;
        }
    }

    flagKnockedBack = false;
}

void CJasonSideview::HandleKeys(DWORD dt)
{
    //Sanh jason dead 
    if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_JASONSIDEVIEW && state == JASONSIDEVIEW_STATE_DEAD)
        return;

    if (CGame::GetInstance()->GetState() == GameState::PLAY_SIDEVIEW_JASON)
    {
        HandleKeysHold(dt);

        auto keyEvents = NewKeyEvents();

        for (auto e : keyEvents)
        {
            int keyCode = e->GetKeyCode();
            if (e->IsDown())
                HandleKeyDown(dt, keyCode);
            else
                HandleKeyUp(dt, keyCode);
        }
    }
    else 
        if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT_JASON ||
            CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT_JASON)
        {
            auto keyEvents = NewKeyEvents();

            for (auto e : keyEvents)
            {
                int keyCode = e->GetKeyCode();
                if (!e->IsDown())
                    HandleKeyUp(dt, keyCode);
            }
        }
}

void CJasonSideview::HandleKeysHold(DWORD dt)
{
    // CuteTN: Auto jump and fire
    if (IsKeyDown(ControlKeys::AutoJumpKey))
        HandleKeyDown(dt, ControlKeys::JumpKey);
    if (IsKeyDown(ControlKeys::AutoFireKey))
        HandleKeyDown(dt, ControlKeys::FireKey);

    if (!flagClimb)
    {
        if (IsKeyDown(DIK_RIGHT))
        {
            flagTurnRight = true;

            if (flagCrawl)// jason crawl right
            {
                SetState(JASONSIDEVIEW_STATE_CRAWL_RIGHT);
                animationHandlers[state]->startLoopIndex = 0;
            }
            else  // jason walk right
            {
                SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
                animationHandlers[state]->startLoopIndex = 0;
            }

            if (flagCrawl && !flagOnAir)
                vx = JASONSIDEVIEW_VX / 2;
            else
            {
                flagJumpWalk = false;
                flagWalk = false; 
                vx = JASONSIDEVIEW_VX;
            }
        }

        if (IsKeyDown(DIK_LEFT))
        {
            flagTurnRight = false;

            // jason crawl left
            if (flagCrawl)
            {
                SetState(JASONSIDEVIEW_STATE_CRAWL_LEFT);
                animationHandlers[state]->startLoopIndex = 0;
            }
            else // jason walk left
            {
                SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
                animationHandlers[state]->startLoopIndex = 0;
            }

            if (flagCrawl && !flagOnAir)
                vx = -JASONSIDEVIEW_VX / 2;
            else
            {
                flagJumpWalk = false;
                flagWalk = false;
                vx = -JASONSIDEVIEW_VX;
            }
        }
    }

    if (IsKeyDown(DIK_DOWN) && flagClimb )
    {
        vy = JASONSIDEVIEW_CLIMB_SPEED;
        animationHandlers[state]->startLoopIndex = 0;
        yStartFalling = y;
    }

    if (IsKeyDown(DIK_UP) && flagClimb)
    {
        vy = -JASONSIDEVIEW_CLIMB_SPEED;
        animationHandlers[state]->startLoopIndex = 0;
        yStartFalling = y;
    }
}

void CJasonSideview::HandleKeyUp(DWORD dt, int keyCode)
{
    if ((keyCode == DIK_RIGHT || keyCode == DIK_LEFT /*|| keyCode == ControlKeys::JumpKey*/) && !flagClimb )
    {
        if ((flagCrawl && !flagOnAir)|| (!flagCrawl && !flagJumpWalk))
            vx = 0;

        if (!flagCrawl && !IsKeyDown(ControlKeys::JumpKey))
        {
            ax = JASONSIDEVIEW_AX*3;
            flagWalk = true;
            /*if (keyCode == DIK_RIGHT)
                SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
            else
                SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);*/
        }

    }

    //stop climbing
    if ((keyCode == DIK_UP || keyCode == DIK_DOWN)&& flagClimb)
    {
        vy = 0;
        animationHandlers[state]->startLoopIndex = 1;
    }

}

void CJasonSideview::HandleKeyDown(DWORD dt, int keyCode)
{

    if (keyCode == DIK_UP)
    {
        if (flagCrawl)
        {
            flagCrawl = false;

            if (flagTurnRight)
                SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
            else
                SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
        }
        else
            if (flagCanClimb && !flagOnAir)
            {
                vx = 0;

                flagClimb = true;
                flagCrawl = false;

                this->x = ladderL - 3;
                this->y = this->y - 2;

                SetState(JASONSIDEVIEW_STATE_CLIMB);
            }
    }

    if (keyCode == DIK_DOWN  && !flagOnAir)
    {
        flagCrawl = true;

        if (overlappingScenePortal)
        {
            LPPORTAL portal = dynamic_cast<LPPORTAL>(overlappingScenePortal);
            if (portal)
            {
                SwitchSceneEvent* sse = new SwitchSceneEvent(portal);
				CGame::AddGameEvent(sse);
            }
        }
        else
        {
            if (flagTurnRight)
                SetState(JASONSIDEVIEW_STATE_CRAWL_RIGHT);
            else
                SetState(JASONSIDEVIEW_STATE_CRAWL_LEFT);
        }
    }

    if (flagCanClimb && keyCode == DIK_DOWN && jason_t < ladderT)
    {
        flagClimb = true;
        flagClimbOver = false;

        vx = 0;

        this->x = ladderL - 3;
        this->y = this->y + 2;

        flagCrawl = false;

        SetState(JASONSIDEVIEW_STATE_CLIMB);
    }

    // jason jump 
    if (keyCode == ControlKeys::JumpKey && !flagOnAir && !flagCrawl && !flagClimb)
    {
        vx = 0;
        vy -= JASONSIDEVIEW_JUMP_SPEED_Y;

        if (flagTurnRight)
            SetState(JASONSIDEVIEW_STATE_JUMP_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_JUMP_LEFT);
    }

    //jason jump when climb
    if (keyCode == ControlKeys::JumpKey && flagClimb && !flagClimbOver)
    {
        vy = -JASONSIDEVIEW_JUMP_SPEED_Y /2 ;
        flagClimb = false;
    }

    if (keyCode == DIK_RSHIFT && !flagOnAir )
    {
        if (CCollisionSolver::IsTouchingSophia(CSophia::GetInstance(), __instance)) 
        {
            vx = 0;
            CJasonJumpInEvent* jasonJumpInEvent = new CJasonJumpInEvent(x, y, currentSectionId);
            CGame::AddGameEvent(jasonJumpInEvent);
        }
    }

    // CuteTN: shoot
    if (keyCode == ControlKeys::FireKey && !flagClimb)
    {
        if(flagBulletReloaded && numberOfJasonSideviewBullets < JASONSIDEVIEW_MAX_BULLETS_ON_CAMERA)
			Shoot();
    }

}

void CJasonSideview::CountJasonSideviewBullets(vector<LPGAMEOBJECT>* coObjs)
{
    numberOfJasonSideviewBullets = 0;
    
    for (auto obj : *coObjs)
    {
        if (dynamic_cast<CBullet_JasonSideview*>(obj))
            numberOfJasonSideviewBullets++;
    }
}

void CJasonSideview::Shoot()
{
    CBullet_JasonSideview* bullet = new CBullet_JasonSideview(0, 0, 0, flagTurnRight ? 1 : -1);
    CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);

    bulletReloadTimer->Start();
    flagBulletReloaded = false;
}

void CJasonSideview::UpdateVelocity(DWORD dt)
{
    BeKnockedBack();
}

void CJasonSideview::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
    if (coEvent == nullptr)
        return;
    if (coEvent->otherObject == this)
        return;

    LPGAMEOBJECT obj = coEvent->otherObject;
    
    if (IsBlockableObject(obj))
    {
        if (coEvent->ny < 0)
        {
            flagOnAir = false;

            if (abs(this->y - yStartFalling) > 2)
                yEndFalling = this->y;

            CheckDistance(yStartFalling, yEndFalling);
        }

        if (flagClimb)
            ;
        else
            CGameObjectBehaviour::BlockObject(dt, coEvent);

        this->allowOverlapWithBlocks = true;
    }

    if (dynamic_cast<LPTILE_AREA>(obj))
    {
        LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);
        
        switch (tileArea->classId)
        {
            flagCanClimb = true;

            case CLASS_TILE_PORTAL:
            {
                if (flagOnAir)
                {
                    CGameObjectBehaviour::BlockObject(dt, coEvent);
                    break;
                }

                LPPORTAL fromPortal = dynamic_cast<LPPORTAL>(obj);
                LPPORTAL toPortal = CPortalLib::GetInstance()->Get(fromPortal->associatedPortalId);

                // Sanh code from here!
                LPGAME_EVENT newEvent = new CWalkInPortalEvent("WalkInPortalEvent", fromPortal, toPortal);
                CGame::GetInstance()->AddGameEvent(newEvent);
                // to do: create an event to CGame, let CGame handle switching section
                DebugOut(L"Jason to portal %d of section %d, tick %d\n", toPortal->associatedPortalId, toPortal->currentSectionId, GetTickCount());
            
                break;
            }

           
        }
    }

    if (dynamic_cast<CEnemy*>(obj))
    {
        CGameGlobal::GetInstance()->beingAttackedByEnemy();
    }

    if (dynamic_cast<CBullet*>(obj))
    {
        CBullet* bullet = dynamic_cast<CBullet*>(obj);
        if (!bullet->isFriendly)
            CGameGlobal::GetInstance()->beingAttackedByEnemy();
    }
}

void CJasonSideview::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x + JASONSIDEVIEW_BOUNDBOX_OFFSETX;
    top = y + JASONSIDEVIEW_BOUNDBOX_OFFSETY;
    right = left + JASONSIDEVIEW_BOUNDBOX_WIDTH;
    bottom = top + JASONSIDEVIEW_BOUNDBOX_HEIGHT;

    if (flagCrawl)
        top += (JASONSIDEVIEW_BOUNDBOX_HEIGHT - JASONSIDEVIEW_CRAWL_BOUNDBOX_HEIGHT);
}

void CJasonSideview::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
    if (flagOnAir)
        yStartFalling = min(yStartFalling, this->y);

    vulnerableFlashingEffect->Update(dt);

    bulletReloadTimer->Update(dt);
    CountJasonSideviewBullets(coObjs);

    // CuteTN Note: Handle knocked back here
    UpdateVelocity(dt);

    this->GetBoundingBox(jason_l, jason_t, jason_r, jason_b);
    if (flagCanClimb)
        if (jason_l > ladderR || jason_r < ladderL || jason_b < ladderT || jason_t > ladderB)
            flagCanClimb = false;

    if (jason_b > ladderB)
    {
        flagClimb = false;
        flagClimbOver = false;
    }

    if (jason_b < ladderT && flagClimb)
    {
        flagClimb = false;
        flagClimbOver = true;
    }

    invulnerableTimer->Update(dt);
    dyingEffectTimer->Update(dt);

    // CuteTN note: we may need to refactor this function
    if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_SOPHIA)
        return;

    HandleKeys(dt);

    if (flagOnAir && vx !=0)
    {
        ax = JASONSIDEVIEW_AX;
        flagJumpWalk = true;
        flagCrawl = false;
    }

    if (vx == 0)
    {
        flagJumpWalk = false;
        flagWalk = false;
    }

    if ((flagJumpWalk || flagWalk) && !flagOnAir) {
        if (flagTurnRight)
        {
            SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
            if (vx > 0)
            {
                vx += ax * dt;
            }
            else
                vx = 0;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
            if (vx < 0)
            {
                vx -= ax * dt;
            }
            else
                vx = 0;
        }

    }

    if (!flagClimb)
    {
        vy += JASONSIDEVIEW_GRAVITY * dt;
    }

    flagOnAir = true;

    if (CGame::GetInstance()->GetState() != GameState::SECTION_SWITCH_LEFT_JASON &&
        CGame::GetInstance()->GetState() != GameState::SECTION_SWITCH_RIGHT_JASON)
    {
		overlappingScenePortal = nullptr;
        ResolveInteractions(dt, coObjs);
    }

    // SANH update cambox camera
    if (x + 16 > camBoxRight) {
        camBoxRight = x + 16;
        camBoxLeft = camBoxRight - 16 * 4;
    }
    else if (x < camBoxLeft) {
        camBoxLeft = x;
        camBoxRight = x + 16 * 4;
    }
    if (y + 16 > camBoxBottom) {
        camBoxBottom = y + 16;
        camBoxTop = camBoxBottom - 16 * 6;
    }
    if (y < camBoxTop) {
        camBoxTop = y;
        camBoxBottom = camBoxTop + 16 * 6;
    }

    if (flagOnAir && !flagClimb)
    {
        if (flagTurnRight)
            SetState(JASONSIDEVIEW_STATE_JUMP_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_JUMP_LEFT);
    }

    if (vy == 0 && vx == 0 && !flagCrawl && !flagClimb && state != JASONSIDEVIEW_STATE_SWIM_RIGHT)
    {
        if (flagTurnRight)
            SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
    }

    //Push vx when jason switch section --- Sanh
    if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT_JASON)
    {
        vx = -JASONSIDEVIEW_VX / 4;
        vy = 0;
        SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
    }
    if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT_JASON)
    {
        vx = JASONSIDEVIEW_VX / 4;
        vy = 0;
        SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
    }

    UpdatePosition(dt);
    if (CGameGlobal::GetInstance()->get_healthJasonSideView() <= 0)
    {
        vx = 0; 
        vy = 0;

        animationHandlers[state]->startLoopIndex = 0;   
        SetState(JASONSIDEVIEW_STATE_DEAD);
        
        if (!dyingEffectTimer->IsRunning())
            dyingEffectTimer->Start();
    }
}

void CJasonSideview::Render(float offsetX, float offsetY)
{
    animationHandlers[state]->Render(x + offsetX, y + offsetY, 255, modifyR, modifyG, modifyB);

    if (!IsKeyDown(DIK_LEFT) && state == JASONSIDEVIEW_STATE_CRAWL_LEFT)
        ;
    else if (!IsKeyDown(DIK_RIGHT) && state == JASONSIDEVIEW_STATE_CRAWL_RIGHT)
        ;
    else
        animationHandlers[state]->Update();
}

CJasonSideview* CJasonSideview::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new CJasonSideview();
    }

    return __instance;
}

CJasonSideview* CJasonSideview::InitInstance(int x, int y, int sectionId)
{
    GetInstance();
    __instance->Init();
    __instance->SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    __instance->SetPosition(x, y);
    __instance->currentSectionId = sectionId;
    __instance->yStartFalling = __instance->yEndFalling = y;

    return __instance;
}

void CJasonSideview::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
    if (!overlappedObj)
        return;

    //Thao vui long code o tren gium chung minh nhe
    if (!flagInvulnerable) {
        if (dynamic_cast<CEnemy*>(overlappedObj))
        {
            CGameGlobal::GetInstance()->beingAttackedByEnemy();
            HandleOnDamage();
            flagKnockedBack = true;
        }

        if (dynamic_cast<CBullet*>(overlappedObj))
        {
            CBullet* bullet = dynamic_cast<CBullet*>(overlappedObj);
            if (!bullet->isFriendly) {
                CGameGlobal::GetInstance()->beingAttackedByBullet();
                HandleOnDamage();
                flagKnockedBack = true;
            }

        }

        if (dynamic_cast<LPTILE_AREA>(overlappedObj))
        {
            LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(overlappedObj);
            if (tileArea->classId == CLASS_TILE_SPIKE)
            {
                CGameGlobal::GetInstance()->beingAttackedBySpike();
                HandleOnDamage();
                flagKnockedBack = true;
            }
        }

        if (dynamic_cast<LPTILE_AREA>(overlappedObj))
        {
            LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(overlappedObj);
            if (tileArea->classId != CLASS_TILE_LAVA)
                ;
            else
            {
                SetState(JASONSIDEVIEW_STATE_SWIM_LEFT);
                CGameGlobal::GetInstance()->beingAttackedByLava();
                HandleOnDamage();
                flagKnockedBack = true;
            }
        }
    }

    if (dynamic_cast<LPTILE_AREA>(overlappedObj))
    {
        LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(overlappedObj);
        if (tileArea->classId == CLASS_TILE_LADDER)
            flagCanClimb = true;
    }

    if (overlappedObj->classId == CLASS_TILE_SCENEPORTAL)
    {
        overlappingScenePortal = overlappedObj;
    }
}

void CJasonSideview::HandleTimerTick(LPTIMER sender)
{
    if (sender == invulnerableTimer)
    {
        flagInvulnerable = false;
        invulnerableTimer->interval = INVULNERABLE_DURATION;
    }

    if (sender == dyingEffectTimer)
    {
        // To do: switch scene
        Sleep(4000);
        CGameEvent* event = new SwitchSceneEvent(ID_SCENE_INTRO);
        CGameGlobal::GetInstance()->resetHealth();
        CGame::AddGameEvent(event);
        dyingEffectTimer->Stop();
    }

    if (sender == bulletReloadTimer)
    {
        flagBulletReloaded = true;
    }
}

CJasonSideview::~CJasonSideview()
{
    DebugOut(L"Thyyyyy cute\n");
    CAnimatableObject::~CAnimatableObject();
}

void CJasonSideview::Start_invulnerableTimer()
{
    flagInvulnerable = true;
    invulnerableTimer = new CTimer(this, INVULNERABLE_DURATION * 4, 1);
    invulnerableTimer->Start();
}