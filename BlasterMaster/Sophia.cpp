#include "Sophia.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include "PortalLib.h"
#include "SophiaAnimationSystem.h"
#include "JasonSideview.h"
#include "JasonJumpOutEvent.h"

CSophia::CSophia(int classId, int x, int y)
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
};

void CSophia::init_camBox()
{
    camBoxLeft = x;
    camBoxRight = x + 16 * 4;
    camBoxTop = y + 32 - 16 * 6;
    camBoxBottom = y + 32 - 1; //Sanh can't explain to you about magic number -1
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

void CSophia::updateBody()
{
    if (flagOnAir) {
        if (vy < 0)
            bodyState = 3;
        else if (vy > 0 && ground-y>1)
            bodyState = 1;
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
        ax = -0.0001;
    }
    if (keyCode == DIK_LEFT)
    {
        // SetState(SOPHIA_STATE_IDLE1_LEFT);
        ax = 0.0001;
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
    if (!flagOnAir &&  keyCode == DIK_RSHIFT)
    {
        CJasonJumpOutEvent* jasonJumpOutEvent = new CJasonJumpOutEvent(x, y, currentSectionId);
        CGame::AddGameEvent(jasonJumpOutEvent);
    }
}
#pragma endregion

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjs)
{
    //SANH-CAMERA
    //don't allow update when player is jason
    if (CGame::GetInstance()->GetCurrentPlayer()->classId == CLASS_JASONSIDEVIEW)
        return;

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

        ResolveInteractions(dt, coObjs);

        UpdatePosition(dt);

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
void CSophia::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
    left = x + SOPHIA_BOUNDBOX_OFFSETX;
    top = y + SOPHIA_BOUNDBOX_OFFSETY;
    right = left + SOPHIA_BOUNDBOX_WIDTH;
    bottom = top + SOPHIA_BOUNDBOX_HEIGHT;
}

void CSophia::Render(float offsetX, float offsetY)
{
    LPSPRITE sprite = CSophiaAnimationSystem::GetInstance()->GetSprite(directionState, gunState, bodyState, wheelState);
    if (sprite)
        if (vy > 0 && bodyState == 1 && ground - y >= 16)
        {
            sprite->Draw(x + offsetX, y - 3 + offsetY);
        }
        else
            sprite->Draw(x + offsetX, y + offsetY);
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
    delete __instance;
    __instance = new CSophia(classId, x, y);
    __instance->currentSectionId = sectionId;

    return __instance;
}
