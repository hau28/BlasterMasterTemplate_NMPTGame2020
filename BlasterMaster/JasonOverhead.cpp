#include "JasonOverhead.h"
#include "TileArea.h"
#include "Portal.h"
#include "GameObjectBehaviour.h"
#include "PortalLib.h"
#include "GameGlobal.h"
#include "WalkInPortalEvent.h"
#include "PlayScene.h"
#include "SwitchSceneEvent.h"
#include "Bullet_JasonOverhead.h"
#include "Grenade_JasonOverhead.h"
#include "Sound.h"

CJasonOverhead* CJasonOverhead::__instance = nullptr;

CJasonOverhead::CJasonOverhead()
{
    Init();
}

void CJasonOverhead::Init()
{
    classId = CLASS_JASONOVERHEAD;
    SetState(JASONOVERHEAD_STATE_IDLE_DOWN);

    // CuteTN Note: DIRTY AF
    LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(JASON_OVERHEAD_ANIMATIONS);
    animationHandlers = objAnims->GenerateAnimationHanlders();

    this->allowOverlapWithBlocks = false;

    SetModifyColor(255, 255, 255, 255);

    invulnerableTimer = new CTimer(this, INVULNERABLE_DURATION, 1);
    invulnerableTimer->Stop();

    dyingEffectTimer = new CTimer(this, DYING_EFFECT_DURATION, 1);
    dyingEffectTimer->Stop();

    flagBulletReloaded = true;
    bulletReloadTimer = new CTimer(this, JASONOVERHEAD_BULLET_RELOAD_DURATION, 1);
    bulletReloadTimer->Stop();

    flaggrenadeReloaded = true;
    grenadeReloadTimer = new CTimer(this, JASONOVERHEAD_GRENADE_RELOAD_DURATION, 1);
    grenadeReloadTimer->Stop();

    vulnerableFlashingEffect = new CObjectFlashingEffectPlayer(this, &flashingColors, JASONOVERHEAD_VULNERABLE_EFFECT_FLASHING_DURATION);
    flagInvulnerable = false;

    flagDead = false;
}


void CJasonOverhead::HandleKeys(DWORD dt)
{
    auto keyEvents = NewKeyEvents();
    //Khong nhan phim khi chuyen section
    if (dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene()))
    {
        CPlayScene* _playScene= dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (_playScene->isSectionSwitch())
        {
            for (auto e : keyEvents)
            {
                int keyCode = e->GetKeyCode();
                    HandleKeyUp(dt, keyCode);
            }
            return;
        }
    }
    HandleKeysHold(dt);

    for (auto e : keyEvents)
    {
        int keyCode = e->GetKeyCode();
        if (e->IsDown())
            HandleKeyDown(dt, keyCode);
        else
            HandleKeyUp(dt, keyCode);
    }
}

void CJasonOverhead::HandleKeyUp(DWORD dt, int keyCode)
{
}

void CJasonOverhead::HandleKeyDown(DWORD dt, int keyCode)
{
    if (flagDead)
        return;

    // if (keyCode == DIK_T)
    // {
    //     CGameGlobal::GetInstance()->beingAttackedByEnemy();
    //     CGameGlobal::GetInstance()->beingAttackedByEnemy();
    //     CGameGlobal::GetInstance()->beingAttackedByEnemy();
    //     CGameGlobal::GetInstance()->beingAttackedByEnemy();
    //     CGameGlobal::GetInstance()->beingAttackedByEnemy();
    // }

    if (keyCode == ControlKeys::FireKey)
    {
        if (flagBulletReloaded && numberOfJasonOverheadBullets < max_bullets_on_cam)
            Shoot();
    }

    if (keyCode == ControlKeys::JumpKey)
    {
        if (flaggrenadeReloaded)
            DropBomb();
    }

    if (keyCode == DIK_P)
    {
        CGameGlobal::GetInstance()-> AddToGunLevel(1);
    }
    if (keyCode == DIK_O)
    {
        CGameGlobal::GetInstance()->AddToGunLevel(-1);
    }
}

void CJasonOverhead::HandleKeysHold(DWORD dt)
{
    MovingDirX = MovingDirY = 0;

    if (flagDead)
        return;

    // CuteTN: Auto jump and fire
    if (IsKeyDown(ControlKeys::AutoJumpKey))
        HandleKeyDown(dt, ControlKeys::JumpKey);
    if (IsKeyDown(ControlKeys::AutoFireKey))
        HandleKeyDown(dt, ControlKeys::FireKey);

    if (IsKeyDown(DIK_UP))
        MovingDirY--;
    if (IsKeyDown(DIK_DOWN))
        MovingDirY++;
    if (IsKeyDown(DIK_LEFT))
        MovingDirX--;
    if (IsKeyDown(DIK_RIGHT))
        MovingDirX++;
}

bool CJasonOverhead::IsMoving()
{
    return (MovingDirX != 0) || (MovingDirY != 0);
}

void CJasonOverhead::UpdateState()
{
    int newState = state;
    if (state == JASONOVERHEAD_STATE_DEAD)
        return;
    if (IsMoving())
    {
        if (MovingDirX != 0 && MovingDirY != 0)
            ;
        else
        {
            if (MovingDirX < 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_LEFT;
                FaceDir = JASON_OVERHEAD_FACEDIR_LEFT;
            }
            else if (MovingDirX > 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_RIGHT;
                FaceDir = JASON_OVERHEAD_FACEDIR_RIGHT;
            }
            else if (MovingDirY < 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_UP;
                FaceDir = JASON_OVERHEAD_FACEDIR_UP;
            }
            else if (MovingDirY > 0)
            {
                newState = JASONOVERHEAD_STATE_WALK_DOWN;
                FaceDir = JASON_OVERHEAD_FACEDIR_DOWN;
            }
        }
    }
    else
    {
        switch (FaceDir)
        {
        case JASON_OVERHEAD_FACEDIR_UP: newState = JASONOVERHEAD_STATE_IDLE_UP; break;
        case JASON_OVERHEAD_FACEDIR_DOWN: newState = JASONOVERHEAD_STATE_IDLE_DOWN; break;
        case JASON_OVERHEAD_FACEDIR_LEFT: newState = JASONOVERHEAD_STATE_IDLE_LEFT; break;
        case JASON_OVERHEAD_FACEDIR_RIGHT: newState = JASONOVERHEAD_STATE_IDLE_RIGHT; break;
        }
    }

    if (CGameGlobal::GetInstance()->get_healthJasonOverHead() <= 0)
    {
        Sound::getInstance()->stop(AREA2);
        newState = JASONOVERHEAD_STATE_DEAD;
        dyingEffectTimer->Start();
        flagDead = true;
    }

    SetState(newState);
}

void CJasonOverhead::GetShootPosition(float& x, float& y, float dx, float dy)
{
    const int JASONOVERHEAD_GUNUP_OFFSETX_FROM_CENTER = 8;
    const int JASONOVERHEAD_GUNUP_OFFSETY_FROM_CENTER = -28;
    const int JASONOVERHEAD_GUNDOWN_OFFSET_FROM_CENTER = -4;
    const int JASONOVERHEAD_GUNLEFTRIGHT_OFFSETY_FROM_CENTER = -15;
    const int JASONOVERHEAD_GUNLEFT_OFFSETX_FROM_CENTER = -12; 
    const int JASONOVERHEAD_GUNTRIGHT_OFFSETX_FROM_CENTER = 12;
    // set the bullet center equals to Sophia center
    CGameObjectBehaviour::CalcBoundingBoxCenter(this, x, y);

    if (dy == 1)
    {
        x += JASONOVERHEAD_GUNDOWN_OFFSET_FROM_CENTER;
    }

    if (dy == -1)
    {
        x += JASONOVERHEAD_GUNUP_OFFSETX_FROM_CENTER;
        y += JASONOVERHEAD_GUNUP_OFFSETY_FROM_CENTER;
    }

    if (dx == -1)
    {
        x += JASONOVERHEAD_GUNLEFT_OFFSETX_FROM_CENTER;
        y += JASONOVERHEAD_GUNLEFTRIGHT_OFFSETY_FROM_CENTER;
    }

    if (dx == 1)
    {
        x += JASONOVERHEAD_GUNTRIGHT_OFFSETX_FROM_CENTER;
        y += JASONOVERHEAD_GUNLEFTRIGHT_OFFSETY_FROM_CENTER;
    }
    
    y += CBullet_JasonOverhead::BULLET_OVERHEAD_OFFSET_FROM_SHADOW;
}

void CJasonOverhead::GetDropBombPosition(float& x, float& y, float dx, float dy)
{
    const int JASONOVERHEAD_GRENADE_OFFSETY_FROM_CENTER = -15;

    CGameObjectBehaviour::CalcBoundingBoxCenter(this, x, y);

    y += JASONOVERHEAD_GRENADE_OFFSETY_FROM_CENTER;
    y += CBullet_JasonOverhead::BULLET_OVERHEAD_OFFSET_FROM_SHADOW;
}

void CJasonOverhead::CountJasonOverheadBullets(vector<LPGAMEOBJECT>* coObjs)
{
    numberOfJasonOverheadBullets = 0;

    for (auto obj : *coObjs)
    {
        if (dynamic_cast<CBullet_JasonOverhead*>(obj))
            numberOfJasonOverheadBullets++;
    }
}

void CJasonOverhead::Shoot()
{
    float dx = 0, dy = 0, sx, sy;

    checkDxDy(dx,dy);

    gunlevel = CGameGlobal::GetInstance()->GetLevelGun();
    Sound::getInstance()->play(JASON_SHOOT_OVERHEAD, false, 1);
    // CBullet_JasonOverhead* bullet = new CBullet_JasonOverhead(x, y, currentSectionId, dx, dy, 0, numberOfJasonOverheadBullets);
    CBullet_JasonOverhead* bullet = new CBullet_JasonOverhead(x, y, currentSectionId, dx, dy, gunlevel, numberOfJasonOverheadBullets);
    if (gunlevel == 3 || gunlevel ==2)
    {
        max_bullets_on_cam = 3;
    }
    else 
        max_bullets_on_cam = 999;

    GetShootPosition(sx, sy, dx,dy);
    CGameObjectBehaviour::SetBoundingBoxCenter(bullet, sx, sy);
    CGameObjectBehaviour::CreateObject(bullet);

    bulletReloadTimer->Start();
    flagBulletReloaded = false;
}

void CJasonOverhead::DropBomb()
{
    float dx = 0, dy = 0, sx, sy;

    checkDxDy(dx, dy);

    if (IsMoving())
        distance = GRENADE_DISTANCE_FAR;
    else
        distance = GRENADE_DISTANCE;
    Sound::getInstance()->play(GRENADE, false, 1);
    CGrenade_JasonOverhead* bomb = new CGrenade_JasonOverhead(x, y, currentSectionId, dx, dy, distance);
    GetDropBombPosition(sx, sy, dx, dy);
    CGameObjectBehaviour::SetBoundingBoxCenter(bomb, sx, sy);
    CGameObjectBehaviour::CreateObject(bomb);

    grenadeReloadTimer->Start();
    flaggrenadeReloaded = false;
    
}

void CJasonOverhead::checkDxDy(float& dx, float& dy)
{
    if (state == JASONOVERHEAD_STATE_IDLE_DOWN || state == JASONOVERHEAD_STATE_WALK_DOWN || state == JASONOVERHEAD_STATE_IDLE_UP || state == JASONOVERHEAD_STATE_WALK_UP)
    {
        if (state == JASONOVERHEAD_STATE_IDLE_DOWN || state == JASONOVERHEAD_STATE_WALK_DOWN)
        {
            dy = 1;
        }
        if (state == JASONOVERHEAD_STATE_IDLE_UP || state == JASONOVERHEAD_STATE_WALK_UP)
        {
            dy = -1;
        }
    }
    if (state == JASONOVERHEAD_STATE_IDLE_LEFT || state == JASONOVERHEAD_STATE_WALK_LEFT)
    {
        dx = -1;
    }
    if (state == JASONOVERHEAD_STATE_IDLE_RIGHT || state == JASONOVERHEAD_STATE_WALK_RIGHT)
    {
        dx = 1;
    }
}

void CJasonOverhead::SnapToPortalMiddle(LPGAMEOBJECT portal, bool snapX)
{
    float oldX = x, oldY = y;
    
    float centerX, centerY;
    CGameObjectBehaviour::CalcBoundingBoxCenter(portal, centerX, centerY);
    CGameObjectBehaviour::SetBoundingBoxCenter(this, centerX, centerY);

    if (snapX)
        y = oldY;
    else
        x = oldX;
}

void CJasonOverhead::HandleTimerTick(LPTIMER sender)
{
    if (sender == invulnerableTimer)
    {
        flagInvulnerable = false;
    }

    if (sender == dyingEffectTimer)
    {
        // To do: switch scene
        // Sleep(4000);
        CGameEvent* event = new SwitchSceneEvent(ID_SCENE_INTRO);
        CGameGlobal::GetInstance()->resetHealth();
        CGame::AddGameEvent(event);
        dyingEffectTimer->Stop();
    }

    if (sender == bulletReloadTimer)
    {
        flagBulletReloaded = true;
    }

    if (sender == grenadeReloadTimer)
    {
        flaggrenadeReloaded = true;
    }
}

CJasonOverhead* CJasonOverhead::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new CJasonOverhead();
    }

    return __instance;
}

CJasonOverhead* CJasonOverhead::InitInstance(int x, int y, int sectionId)
{
    GetInstance();
    __instance->Init();
    __instance->SetPosition(x, y);
    __instance->UpdateState();
    __instance->currentSectionId = sectionId;

    return __instance;
}

void CJasonOverhead::HandleOnDamage()
{
    if (CGameGlobal::GetInstance()->get_healthJasonSideView() > 0) {
        Sound::getInstance()->play(JASON_GOT_HIT_OVERHEAD, false, 1);
    }
    flagInvulnerable = true;
    invulnerableTimer->Start();
    PlayVulnerableFlasingEffect();
}

void CJasonOverhead::PlayVulnerableFlasingEffect()
{
    if (vulnerableFlashingEffect)
        vulnerableFlashingEffect->Play();
}

void CJasonOverhead::UpdateVelocity(DWORD dt)
{
    vx = MovingDirX * JASON_OVERHEAD_MOVING_SPEED_X * dt;
    vy = MovingDirY * JASON_OVERHEAD_MOVING_SPEED_Y * dt;
}

void CJasonOverhead::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
    if (coEvent == nullptr)
        return;
    if (coEvent->otherObject == this)
        return;

    LPGAMEOBJECT obj = coEvent->otherObject;

    if (IsBlockableObject(obj))
    {
        CGameObjectBehaviour::BlockObject(dt, coEvent);
    }

    if (dynamic_cast<LPTILE_AREA>(obj))
    {
        LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);

        switch (tileArea->classId)
        {
        case CLASS_TILE_PORTAL:
        {
            if (currentSectionId == CGameGlobal::GetInstance()->ID_SECTION_BOSSOVERHEAD && !CGameGlobal::GetInstance()->isDeadBoss)
            {
                CGameObjectBehaviour::BlockObject(dt, coEvent);
            }
            else
            {
                SnapToPortalMiddle(obj, coEvent->ny != 0);

                // CuteTN: this fixes a bug when JS enters a portal
                if (coEvent->nx)
                    vy = MovingDirY = 0;
                else
                    vx = MovingDirX = 0;

                LPPORTAL fromPortal = dynamic_cast<LPPORTAL>(obj);
                LPPORTAL toPortal = CPortalLib::GetInstance()->Get(fromPortal->associatedPortalId);

                // Sanh code from here!
                LPGAME_EVENT newEvent = new CWalkInPortalEvent("WalkInPortalEvent", fromPortal, toPortal);

                CGame::GetInstance()->AddGameEvent(newEvent);
                // to do: create an event to CGame, let CGame handle switching section
                DebugOut(L"Jason to portal %d of section %d, tick %d\n", toPortal->associatedPortalId, toPortal->currentSectionId, GetTickCount());
            }

            break;
        }

        case CLASS_TILE_SCENEPORTAL:
        {
            LPPORTAL portal = dynamic_cast<LPPORTAL>(obj);
            if (portal)
            {
                Sound::getInstance()->play(SWITCH_SCENE, false, 1);
                SwitchSceneEvent* sse = new SwitchSceneEvent(portal);
                DebugOut(L"Switch to scene Id: %d\n", sse->getIDNextScene());
                CGame::AddGameEvent(sse);
            }

            break;
        }

        }
    }
}

void CJasonOverhead::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
    if (!overlappedObj)
        return;

    if (!flagInvulnerable) {
        if (dynamic_cast<CEnemy*>(overlappedObj))
        {
            CGameGlobal::GetInstance()->beingAttackedByEnemy();
            HandleOnDamage();
        }

        if (dynamic_cast<CBullet*>(overlappedObj))
        {
            CBullet* bullet = dynamic_cast<CBullet*>(overlappedObj);
            if (!bullet->isFriendly) {
                CGameGlobal::GetInstance()->beingAttackedByBullet();
                HandleOnDamage();
            }

        }

        if (dynamic_cast<LPTILE_AREA>(overlappedObj))
        {
            LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(overlappedObj);
            if (tileArea->classId == CLASS_TILE_SPIKE)
            {
                CGameGlobal::GetInstance()->beingAttackedBySpike();
                HandleOnDamage();
            }
        }
    }


    if (!flagInvulnerable)
    {
        if (dynamic_cast<CEnemy*>(overlappedObj))
        {
            CGameGlobal::GetInstance()->beingAttackedByEnemy();
        }

        if (dynamic_cast<CBullet*>(overlappedObj))
        {
            CBullet* bullet = dynamic_cast<CBullet*>(overlappedObj);
            if (!bullet->isFriendly)
                CGameGlobal::GetInstance()->beingAttackedByEnemy();
        }
    }
}

void CJasonOverhead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
    dyingEffectTimer->Update(dt);
    bulletReloadTimer->Update(dt);
    grenadeReloadTimer->Update(dt);
    CountJasonOverheadBullets(coObjs);

    invulnerableTimer->Update(dt);
    vulnerableFlashingEffect->Update(dt);

    HandleKeys(dt);
    CAnimatableObject::Update(dt, coObjs);
    UpdateState();

    //SANH
    GameState _gameState = CGame::GetInstance()->GetState();
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_DOWN)
        SetSpeed(0, 0.1);
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_LEFT)
        SetSpeed(-0.1, 0);
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_RIGHT)
        SetSpeed(0.1, 0);
    if (_gameState == GameState::SECTION_SWITCH_OVERHEAD_UP)
        SetSpeed(0, -0.1);      
}

void CJasonOverhead::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x + JASONOVERHEAD_BOUNDBOX_OFFSETX;
    top = y + JASONOVERHEAD_BOUNDBOX_OFFSETY;
    right = left + JASONOVERHEAD_BOUNDBOX_WIDTH;
    bottom = top + JASONOVERHEAD_BOUNDBOX_HEIGHT;
}
