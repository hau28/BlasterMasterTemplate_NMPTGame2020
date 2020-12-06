#include "JasonSideview.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include "Sophia.h"

#include <algorithm>
#include <assert.h>
CJasonSideview* CJasonSideview::__instance = nullptr;

CJasonSideview::CJasonSideview()
{
    classId = CLASS_JASONSIDEVIEW;
    SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    // CuteTN Note: DIRTY AF
    LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(JASONSIDEVIEW_ANIMATIONS);
    animationHandlers = objAnims->GenerateAnimationHanlders();
}

CJasonSideview::CJasonSideview(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId)
{
    SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
};

#pragma region key events handling

void CJasonSideview::HandleKeys(DWORD dt)
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

void CJasonSideview::HandleKeysHold(DWORD dt)
{
    
    if (IsKeyDown(DIK_RIGHT))
    {
        Jason_turnRight = true;

        if (flag_keydown)
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_RIGHT);
            animationHandlers[state]->startLoopIndex = 0;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
            animationHandlers[state]->startLoopIndex = 0;
        }

        if (flag_keydown && vy == 0 && !flagOnAir)
            vx = JASONSIDEVIEW_VX / 2;
        else
            vx = JASONSIDEVIEW_VX;

    };

    if (IsKeyDown(DIK_LEFT))
    {
        Jason_turnRight = false; 

        if (flag_keydown)
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_LEFT);
            animationHandlers[state]->startLoopIndex = 0;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
            animationHandlers[state]->startLoopIndex = 0;
        }

        if (flag_keydown && vy == 0 && !flagOnAir)
            vx = -JASONSIDEVIEW_VX / 2;
        else
            vx = -JASONSIDEVIEW_VX;

    }
}

void CJasonSideview::HandleKeyUp(DWORD dt, int keyCode)
{
    if (keyCode == DIK_RIGHT || keyCode == DIK_LEFT)
    {

        if (flagOnAir)
        {
            ax = JASONSIDEVIEW_AX;
            flag_jumpwalk = true;
        }
        else 
            vx = 0; 

        if (!flag_keydown && !IsKeyDown(DIK_X))
        {
            if (keyCode == DIK_RIGHT)
            {
                Jason_turnRight == true;
                SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
            }
            else
            {
                Jason_turnRight == false;
                SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
            };
        }

        if (flag_keydown)
        {
            if (keyCode == DIK_RIGHT)
            {
                Jason_turnRight == true;
                animationHandlers[state]->startLoopIndex = 1;
            }
            else
            {
                Jason_turnRight == false;
                animationHandlers[state]->startLoopIndex = 1;
            }
        }
    }

	if (keyCode == DIK_UP || keyCode == DIK_DOWN)
		vy = 0;
}

void CJasonSideview::HandleKeyDown(DWORD dt, int keyCode)
{
    if (keyCode == DIK_RIGHT || keyCode == DIK_LEFT)
        vx = 0;

    if (keyCode == DIK_DOWN && vy == 0)
    {
        flag_keydown = true;

        if (Jason_turnRight)
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_RIGHT);
            animationHandlers[state]->startLoopIndex = 1;
        }
        else
        {
            SetState(JASONSIDEVIEW_STATE_CRAWL_LEFT);
            animationHandlers[state]->startLoopIndex = 1;
        }
    }

    if (keyCode == DIK_UP && flag_keydown)
    {
        vy = 0;
        flag_keydown = false;

        if (Jason_turnRight)
            SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
    } 

    if (keyCode == DIK_X && !flagOnAir && !flag_keydown)
    {
        vx = 0;
        vy -= JASONSIDEVIEW_JUMP_SPEED_Y;

        if (Jason_turnRight)
            SetState(JASONSIDEVIEW_STATE_JUMP_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_JUMP_LEFT);
        
    }
}

void CJasonSideview::UpdateVelocity(DWORD dt)
{

}

void CJasonSideview::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
            CGameObjectBehaviour::GetBlocked(dt, coEvent);

            if (coEvent->ny < 0)
                flagOnAir = false;

            break;
        }

        case CLASS_TILE_PORTAL:
        {
            LPPORTAL fromPortal = dynamic_cast<LPPORTAL>(obj);
            LPPORTAL toPortal = CPortalLib::GetInstance()->Get(fromPortal->associatedPortalId);

            // Sanh code from here!
            LPGAME_EVENT newEvent = new CWalkInPortalEvent("WalkInPortalEvent", fromPortal, toPortal);
            CGame::GetInstance()->AddGameEvent(newEvent);
            // to do: create an event to CGame, let CGame handle switching section
            DebugOut(L"To portal %d of section %d\n", toPortal->associatedPortalId, toPortal->currentSectionId);
        }
        }
    }
}

void CJasonSideview::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x + JASONSIDEVIEW_BOUNDBOX_OFFSETX;
    top = y + JASONSIDEVIEW_BOUNDBOX_OFFSETY;
    right = left + JASONSIDEVIEW_BOUNDBOX_WIDTH;
    bottom = top + JASONSIDEVIEW_BOUNDBOX_HEIGHT;
}

void CJasonSideview::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
    HandleKeys(dt);

    // dơ lắm cơ mà fix sau đi :((( 
    if (vx == 0) flag_jumpwalk = false;

    if (flag_jumpwalk && !flagOnAir ) {
        if (Jason_turnRight) 
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
                vx -=  ax * dt;
            }
            else
                vx = 0;
        }   
        
    }

    vy += JASONSIDEVIEW_GRAVITY * dt;
    flagOnAir = true;

    CAnimatableObject::Update(dt, coObjs);

    if (flagOnAir && Jason_turnRight)
        SetState(JASONSIDEVIEW_STATE_JUMP_RIGHT);

    if (flagOnAir && !Jason_turnRight)
        SetState(JASONSIDEVIEW_STATE_JUMP_LEFT);

    if (vy == 0 && vx == 0 && !flag_keydown)
    {
        if (Jason_turnRight)
            SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
        else
            SetState(JASONSIDEVIEW_STATE_IDLE_LEFT);
    }

    //if (CCollisionSolver::IsOverlapped(CJasonSideview,))
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
    __instance->SetState(JASONSIDEVIEW_STATE_IDLE_RIGHT);
    __instance->SetPosition(x, y);
    __instance->currentSectionId = sectionId;

    return __instance;
}


