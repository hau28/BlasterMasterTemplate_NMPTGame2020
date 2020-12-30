#include "Sophia.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include "PortalLib.h"
#include "SophiaAnimationSystem.h"
#include "JasonSideview.h"
#include "JasonJumpOutEvent.h"
#include "CreateObjectEvent.h"
#include "Bullet_Sophia.h"
#include "Bullet_HomingMissile.h"
#include "Bullet_MultiwarheadMissile.h"
#include "Bullet_ThunderBreak.h"
#include "SwitchSceneEvent.h"
#include "GameGlobal.h"

CSophia::CSophia(int classId, int x, int y)
{
    Init(classId, x, y);
}

void CSophia::Init(int classId, int x, int y)
{
    this->classId = classId;
    SetPosition(x, y);
    directionState = 1;
    turnRight = true;
    gunState = wheelState = 0;
    vyMax = SOPHIA_MAX_FALL_SPEED;
    vxMax = SOPHIA_MAX_SPEED;
    lastTimeupdateDirection = GetTickCount();
    lastTimeupdateGun = GetTickCount();
    camBoxLeft = x;
    camBoxRight = x + 16 * 4;
    camBoxTop = y + 32 - 16 * 6;
    camBoxBottom = y + 32;
    portaling = 0;
    invulnerableTimer = new CTimer(this, INVULNERABLE_DURATION, 1);
    invulnerableTimer->Stop();
    flagInvulnerable = false;

    // CuteTN Test
    SetModifyColor(255, 255, 255);

    dyingEffectTimer = new CTimer(this, DYING_EFFECT_DURATION, 1);
    dyingEffectTimer->Stop();

    vulnerableFlashingEffect = new CObjectFlashingEffectPlayer(this, &flashingColors, SOPHIA_VULNERABLE_EFFECT_FLASHING_DURATION);

    flagDead = false;
};

void CSophia::setGunState(int state) {
    gunState = state;
}

void CSophia::init_camBox()
{
    camBoxLeft = x;
    camBoxRight = x + 16 * 4;
    camBoxTop = y + 32 - 16 * 6;
    camBoxBottom = y + 32 - 1; //Sanh can't explain to you about magic number -1
}

void CSophia::init_camBox_FollowCamera()
{
    float cameraX, cameraY;
    CGame::GetInstance()->GetCamPos(cameraX, cameraY);
    float centerPointX = cameraX + CGame::GetInstance()->GetScreenWidth() / 2 + 8;
    float centerPointY = cameraY + CGame::GetInstance()->GetScreenHeight() / 2  + 16;

    camBoxLeft = centerPointX - 16 * 2;
    camBoxRight = camBoxLeft + 16 * 5;
    camBoxBottom = centerPointY + 16 * 3;
    camBoxTop = camBoxBottom - 16 * 6;
}

#pragma region key events handling

void CSophia::HandleKeys(DWORD dt)
{
    //SANH-SWITCH SECTION
    if (CGame::GetInstance()->GetState() == GameState::PLAY_SIDEVIEW_SOPHIA)
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
    //SANH - Only allow handleKeyUp when sophia switch section
    if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT ||
        CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT)
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

void CSophia::updateWheel()
{

    if ((vx >= 0.01 && !(flagOnAir && !IsKeyDown(DIK_RIGHT)))|| portaling)
    {
        if (GetTickCount() - lastTimeupdateWheel >= (300 - 2000 * abs(vx)) / 5)
        {
            wheelState = (wheelState + 1) % 4;
            if (wheelState % 2 == 0)
            {
                if (bodyState == 2)
                    bodyState = 1;
                else if (bodyState == 1)
                    bodyState = 2;
            }
            lastTimeupdateWheel = GetTickCount();
        }
    }
    if ((vx <= -0.01 && !(flagOnAir && !IsKeyDown(DIK_LEFT)))||portaling)
    {
        if (GetTickCount() - lastTimeupdateWheel >= (300 - 2000 * abs(vx)) / 5)
        {
            wheelState = (wheelState + 1) % 4;
            if (wheelState % 2 == 0)
            {
                if (bodyState == 2)
                    bodyState = 1;
                else if (bodyState == 1)
                    bodyState = 2;
            }
            lastTimeupdateWheel = GetTickCount();
        }
    }
    if (vx == 0)
        bodyState = 2;
}

void CSophia::updateDirection()
{
        if (turnRight)
        {
            if (GetTickCount() - lastTimeupdateDirection > 100)
            {
                if (directionState < 3)
                {
                    directionState++;
                    lastTimeupdateDirection = GetTickCount();
                }
            }
        }
        else
        {
            if (GetTickCount() - lastTimeupdateDirection > 100)
            {
                if (directionState > 0)
                {
                    directionState--;
                    lastTimeupdateDirection = GetTickCount();
                }
            }
        }
}

void CSophia::updateGun()
{
    if (IsKeyDown(DIK_UP) && !portaling)
    {
        if (gunState == 0)
        {
            gunState++;
            lastTimeupdateGun = GetTickCount();
        }
        else if (gunState < 2)
        {
            if (GetTickCount() - lastTimeupdateGun > 100)
            {
                gunState++;
                lastTimeupdateGun = GetTickCount();
            }
        }
    }
    else if (!IsKeyDown(DIK_UP))
    {
        if (gunState == 2)
        {
            gunState--;
            lastTimeupdateGun = GetTickCount();
        }
        else if (gunState > 0)
            if (GetTickCount() - lastTimeupdateGun > 100)
            {
                gunState--;
                lastTimeupdateGun = GetTickCount();
            }
    }
}

void CSophia::PlayVulnerableFlasingEffect()
{
    if (vulnerableFlashingEffect)
        vulnerableFlashingEffect->Play();
}

void CSophia::HandleOnDamage()
{
    flagInvulnerable = true;
    invulnerableTimer->Start();
    vulnerableFlashingEffect->Play();
}

void CSophia::BeKnockedBack()
{
    if (!flagKnockedBack)
        return;

    vy -= SOPHIA_KNOCKEDBACK_VY;

    if (directionState <= 1)
        vx += SOPHIA_KNOCKEDBACK_VX;
    else
        vx -= SOPHIA_KNOCKEDBACK_VX;

    flagKnockedBack = false;
}

void CSophia::updateBody()
{
    if (flagOnAir) {
        if (vy < 0)
            bodyState = 3;
        else if (vy > 0 && ground-y>1)
            bodyState = 1;
    }

    if (flag_JasonJumpOut) 
    {
        //jasonJumpIn();
        bodyState = 4;
        if (GetTickCount() - lastTimeupdateGun > 200)
        {
            bodyState = 2;
            lastTimeupdateGun = GetTickCount();
            //flag_JasonJumpOut = false;
        }
  
    }
}

void CSophia::HandleKeysHold(DWORD dt)
{

    if (IsKeyDown(DIK_RIGHT))
    {
        // SetState(SOPHIA_STATE_WALK_RIGHT);
        if (directionState != 3 && !portaling)
        {
            turnRight = true;
        }
        ax = SOPHIA_ENGINE;
    }
    else if (IsKeyDown(DIK_LEFT))
    {
        // SetState(SOPHIA_STATE_WALK_LEFT);
        if (directionState != 0 && !portaling)
        {
            turnRight = false;
        }

        ax = -SOPHIA_ENGINE;
    }

    if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_UP) && IsKeyDown(DIK_RIGHT))
    {
        /*
		if (!isLeft && state != SOPHIA_STATE_GUNUP_RIGHT)
		{
			SetState(SOPHIA_STATE_GUNUP_RIGHT);
			animationHandlers[state]->currentFrameIndex = 0;
		}
		if (isLeft && state != SOPHIA_STATE_GUNUP_LEFT)
		{
			SetState(SOPHIA_STATE_GUNUP_LEFT);
			animationHandlers[state]->currentFrameIndex = 0;
		}
		*/
    }
    if (IsKeyDown(DIK_DOWN))
    {
    }
}
void CSophia::HandleKeyUp(DWORD dt, int keyCode)
{
    if (keyCode == DIK_RIGHT)
    {
        // SetState(SOPHIA_STATE_IDLE1_RIGHT);
        ax = -SOPHIA_AX;
    }
    if (keyCode == DIK_LEFT)
    {
        // SetState(SOPHIA_STATE_IDLE1_LEFT);
        ax = SOPHIA_AX;
    }

    if (vy < 0 && ground - y > 48.5 && keyCode == DIK_X)
        vy = 0;
}

void CSophia::HandleKeyDown(DWORD dt, int keyCode)
{
    if (!flagOnAir && keyCode == DIK_X)
    {
        vy = -SOPHIA_JUMP_FORCE;
    }
    if (!flagOnAir && keyCode == DIK_RSHIFT)
    {
        flag_JasonJumpOut = true;
        updateBody();
        
        float xCenter, yCenter;
        CGameObjectBehaviour::CalcBoundingBoxCenter(CSophia::GetInstance(), xCenter, yCenter);
        CJasonJumpOutEvent* jasonJumpOutEvent = new CJasonJumpOutEvent(xCenter, yCenter, currentSectionId);
        CGame::AddGameEvent(jasonJumpOutEvent);
        //CSophia::GetInstance()->init_camBox_FollowCamera();
        vx = 0;
        ax = 0;
    }
    //SANH_SWITCH SCENE
    //Help Sanh fastly test switch scene
    if (keyCode == DIK_SPACE)
    {
        CGameEvent* event = new SwitchSceneEvent(ID_SCENE_END);
        CGame::AddGameEvent(event);
    }

    // CuteTN Bullet
    if (keyCode == DIK_C)
    {
        if (IsKeyDown(DIK_DOWN))
            ShootWeapon();
        else
			Shoot();
    }

    // CuteTN switch scene through scene portal
    if (keyCode == DIK_DOWN && overlappingScenePortal)
    {
        LPPORTAL portal = dynamic_cast<LPPORTAL>(overlappingScenePortal);
        if (portal)
        {
            SwitchSceneEvent* sse = new SwitchSceneEvent(portal);
            if(sse->getIDNextScene() == ID_SCENE_END)
				CGame::AddGameEvent(sse);
        }
    }

}
#pragma endregion

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjs)
{
    vulnerableFlashingEffect->Update(dt);

    //SANH-CAMERA
    //don't allow update when player is jason
    invulnerableTimer->Update(dt);
    dyingEffectTimer->Update(dt);

    this->coObjects = coObjs;

    if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_JASONSIDEVIEW && bodyState == 2)
    {
        flag_JasonJumpOut = false;
        return;
    }
    // dirty demo
    HandleKeys(dt);

    portaling = 0;
    if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT)
        portaling = 1;
    if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT)
        portaling = 2;
    if (!portaling) {

        updateGun();
        UpdateVelocity(dt);
        updateWheel();
        updateDirection();
        updateBody();
        
        flagOnAir = true;
		overlappingScenePortal = nullptr;

        ResolveInteractions(dt, coObjs);//

        UpdatePosition(dt);//

        //update camBox
        if (x + 16 * 2 >= camBoxRight) {
            camBoxRight = x + 16 * 2;
            camBoxLeft = x - 16 * 2;
        }
        else if (x <= camBoxLeft) {
            camBoxLeft = x;
            camBoxRight = x + 16 * 4;
        }
        if (y + 32 >= camBoxBottom) {
            camBoxBottom = y + 32;
            camBoxTop = camBoxBottom - 16 * 6;
        }
        if (y - 16 <= camBoxTop) {
            camBoxTop = y - 16;
            camBoxBottom = camBoxTop + 16 * 6;                                                                          
        }

        if (!flagOnAir)
            ground = y;
    }
    else if (portaling==2) {
        vx = SOPHIA_MAX_SPEED/3;
        vy = 0;
        UpdatePosition(dt);
        updateDirection();
        updateWheel();
    }
    else {
        vx = -SOPHIA_MAX_SPEED / 3;
        vy = 0;
        UpdatePosition(dt);
        updateDirection();
        updateWheel();
    }

    if ((!flagDead) && (!flagOnAir) && CGameGlobal::GetInstance()->get_healthSophia() <= 0)
    {
        vx = 0;
        vy = 0;

        if (!dyingEffectTimer->IsRunning())
            dyingEffectTimer->Start();

        // CuteTN To do: Sophia's explosion
        Explode();

        flagDead = true;
    }

}

void CSophia::Explode()
{
    const float SOPHIA_EXPLOSION_OFFSET_X = -9;
    const float SOPHIA_EXPLOSION_OFFSET_Y = -8;

    int explosionX = x + SOPHIA_EXPLOSION_OFFSET_X;
    int explosionY = y + SOPHIA_EXPLOSION_OFFSET_Y;

	CGameObjectBehaviour::CreateExplosion(CLASS_SOPHIA_EXPLOSION, explosionX, explosionY, currentSectionId);
}

void CSophia::UpdateVelocity(DWORD dt)
{
    //jump handler
    if (vy < 0 && !IsKeyDown(DIK_X) && ground - y > 48 && ground - y < 48.5)
        vy = 0;

    vy += SOPHIA_GRAVITY * dt;
    vy = min(vy, vyMax);

    vx += ax * dt;
    vx = min(vx, vxMax);
    vx = max(vx, -vxMax);

    //friction handler
    if (directionState == 2 && turnRight)
    {
        if (ax <= 0)
        {
            if (flagOnAir)
                ax = 0;
            else
                ax = -FRICTION;
            if (vx <= 0)
            {
                vx = 0;
                ax = 0;
            }
        }
    }
    else if (directionState == 1 && !turnRight)
    {
        if (ax >= 0)
        {
            if (flagOnAir)
                ax = 0;
            else
                ax = FRICTION;
            if (vx >= 0)
            {
                vx = 0;
                ax = 0;
            }
        }
    }
    else if (directionState == 3 || directionState == 1)
    {
        if (ax <= 0)
        {
            if (flagOnAir)
                ax = 0;
            else
                ax = -FRICTION;
            if (vx <= 0)
            {
                vx = 0;
                ax = 0;
            }
        }
    }
    else if (directionState == 0 || directionState == 2)
    {
        if (ax >= 0)
        {
            if (flagOnAir)
                ax = 0;
            else
                ax = FRICTION;
            if (vx >= 0)
            {
                vx = 0;
                ax = 0;
            }
        }
    }

    BeKnockedBack();
    // vx += ax
}

void CSophia::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{

    if (coEvent == nullptr)
        return;
    if (coEvent->otherObject == this)
        return;

    LPGAMEOBJECT obj = coEvent->otherObject;

    if (dynamic_cast<LPTILE_AREA>(obj))
    {
        LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);

        switch (tileArea->classId)
        {
        case CLASS_TILE_BLOCKABLE:
        {
            CGameObjectBehaviour::BlockObject(dt, coEvent);

            if (coEvent->ny < 0)
                flagOnAir = false;


            break;
        }

        case CLASS_TILE_PORTAL:
        {
            // CuteTN Note: if the player is on air, do not invoke walk in portal event
            // instead, portal behave just like a blockable tile
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
            DebugOut(L"Sophia to portal %d of section %d\n", fromPortal->associatedPortalId, toPortal->currentSectionId);
        }
        }
    }

}

void CSophia::HandleOverlap(LPGAMEOBJECT overlappedObj)
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
                CGameGlobal::GetInstance()->beingAttackedByEnemy();
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
            }
        }

        if (dynamic_cast<LPTILE_AREA>(overlappedObj))
        {
            LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(overlappedObj);
            if (tileArea->classId == CLASS_TILE_LAVA)
            {
                CGameGlobal::GetInstance()->beingAttackedByLava();
                HandleOnDamage();
            }
        }
    }

    if (overlappedObj->classId == CLASS_TILE_SCENEPORTAL)
    {
        overlappingScenePortal = overlappedObj;
    }
}
void CSophia::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
    float offsetX, offsetY, width, height;
    bool isTurnedLeft = directionState <= 1;

    CGameObjectBehaviour::TransformBoundBox
    (
        SOPHIA_BOUNDBOX_OFFSETX, SOPHIA_BOUNDBOX_OFFSETY, SOPHIA_BOUNDBOX_WIDTH, SOPHIA_BOUNDBOX_HEIGHT, SOPHIA_SPRITE_WIDTH, SOPHIA_SPRITE_HEIGHT,
        offsetX, offsetY, width, height,
        isTurnedLeft, false
    );

    left = x + offsetX;
    top = y + offsetY;
    right = left + width;
    bottom = top + height;
}

void CSophia::Render(float offsetX, float offsetY)
{
    if (flagDead)
        return;

    LPSPRITE sprite = CSophiaAnimationSystem::GetInstance()->GetSprite(directionState, gunState, bodyState, wheelState);
    if (sprite)
        if (vy > 0 && bodyState == 1 && ground - y >= 16)
        {
            sprite->Draw(x + offsetX, y - 3 + offsetY, 255, false, 0, modifyR, modifyG, modifyB);
        }
        else
            sprite->Draw(x + offsetX, y + offsetY, 255, false, 0, modifyR, modifyG, modifyB);
}

CSophia *CSophia::__instance = nullptr;

CSophia *CSophia::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new CSophia();
    }
    return __instance;
}

CSophia *CSophia::InitInstance(int classId, int x, int y, int sectionId)
{
    GetInstance();
    __instance->Init(classId, x, y);
    //__instance->classId = classId;
    //__instance->SetPosition(x, y);
    __instance->currentSectionId = sectionId;

    return __instance;
}

void CSophia::GetShootPosition(float& x, float& y)
{
    const int SOPHIA_GUN_OFFSETY_FROM_CENTER = -5;

    const int SOPHIA_GUNUPLEFT_OFFSETX_FROM_CENTER = 6;  // Idk why these 2 numbers are not equal, but here we go :)
    const int SOPHIA_GUNUPRIGHT_OFFSETX_FROM_CENTER = -4; //

    // set the bullet center equals to Sophia center
    CGameObjectBehaviour::CalcBoundingBoxCenter(this, x, y);

    y += SOPHIA_GUN_OFFSETY_FROM_CENTER;

    if (gunState == 2)
        if(directionState <= 1)
			x += SOPHIA_GUNUPLEFT_OFFSETX_FROM_CENTER;
		else
			x += SOPHIA_GUNUPRIGHT_OFFSETX_FROM_CENTER;
}

void CSophia::GetGunDirection(float& dirX, float& dirY)
{
    if (gunState == 2)
    {
        dirX = 0;
        dirY = -1;
        return;
    }

    dirY = 0;
    dirX = directionState <= 1 ? -1 : 1;
}

// CuteTN: Sophia shoot
void CSophia::Shoot()
{
    float dx, dy, sx, sy;
    GetGunDirection(dx, dy);

    LPBULLET_SOPHIA bullet = new CBullet_Sophia(x, y, currentSectionId, dx, dy);

    GetShootPosition(sx, sy);
    CGameObjectBehaviour::SetBoundingBoxCenter(bullet, sx, sy);

    CGameObjectBehaviour::CreateObject(bullet);
}

void CSophia::ShootWeapon()
{
    switch (CGameGlobal::GetInstance()->selectedWeapon)
    {
    case TypeWeapons::HomingMissile:
        ShootHomingMissile();
        break;
    case TypeWeapons::ThunderBreak:
        ShootThunderBreak();
        break;
    case TypeWeapons::MultiwarheadMissile:
        ShootMultiwarheadMissile();
        break;
    }
}

void CSophia::ShootHomingMissile()
{
    int usesLeft = CBullet_HomingMissile::MAX_HOMING_MISSILE_PER_USE;

	LPGAMEOBJECT targetObject = nullptr;

	// finding target
    for (auto obj : *coObjects)
    {
        if (usesLeft == 0)
            return;

		if (!CGameGlobal::GetInstance()->CheckSophiaCanUseWeapon())
			continue;

        if (checkObjInCamera(obj) && dynamic_cast<CEnemy*>(obj))
        {
            targetObject = obj;

			if (!targetObject)
				continue;

			CBullet_HomingMissile* bullet = new CBullet_HomingMissile(0, 0, 0, targetObject);
			CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);

			// CuteTN Note: believe me, it counts for each missile :)
			CGameGlobal::GetInstance()->AddToSelectedWeapon(-1);
            usesLeft--;
        }
    }
}

void CSophia::ShootMultiwarheadMissile()
{
    bool initLeft = directionState <= 1;
    vector<float> initBulletVys =
    {
        0,
        - CBullet_MultiwarheadMissile::MULTIWARHEAD_MISSILE_INIT_VELOCITY_Y_TOP,
        CBullet_MultiwarheadMissile::MULTIWARHEAD_MISSILE_INIT_VELOCITY_Y_TOP,
    };

    // CuteTN have fun:
    //for (float initVy = -0.1; initVy <= 0.1; initVy += 0.02)
    for (auto initVy : initBulletVys)
    {
		if (!CGameGlobal::GetInstance()->CheckSophiaCanUseWeapon())
			continue;

		CBullet_MultiwarheadMissile* bullet = new CBullet_MultiwarheadMissile(0, 0, 0, initLeft, 0, initVy);
		CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);

		// CuteTN Note: believe me, it counts for each missile :)
		//CGameGlobal::GetInstance()->AddToSelectedWeapon(-1);
	}

}

void CSophia::ShootThunderBreak()
{
    if (!CGameGlobal::GetInstance()->CheckSophiaCanUseWeapon())
        return;

    CBullet_ThunderBreak* bullet = new CBullet_ThunderBreak(0, 0, 0);
    CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);

    CGameGlobal::GetInstance()->AddToSelectedWeapon(-1);
}

void CSophia::HandleTimerTick(LPTIMER sender)
{
    if (sender == invulnerableTimer)
    {
        flagInvulnerable = false;
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
}

CSophia::~CSophia()
{
    DebugOut(L"Thy cuteeee\n");
    CAnimatableObject::~CAnimatableObject();
}
