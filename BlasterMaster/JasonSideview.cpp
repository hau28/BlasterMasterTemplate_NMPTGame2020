#include "JasonSideview.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"

CJasonSideview* CJasonSideview::__instance = nullptr;

CJasonSideview::CJasonSideview()
{
    classId = CLASS_JASONSIDEVIEW;
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
        SetState(JASONSIDEVIEW_STATE_WALK_RIGHT);
	}
    else if (IsKeyDown(DIK_LEFT))
    {
        SetState(JASONSIDEVIEW_STATE_WALK_LEFT);
    }
	if (IsKeyDown(DIK_UP))
	{
	
	}
	if (IsKeyDown(DIK_DOWN))
	{
	
	}
}
void CJasonSideview::HandleKeyUp(DWORD dt, int keyCode)
{
	if (keyCode == DIK_RIGHT || keyCode == DIK_LEFT)
		vx = 0;

	if (keyCode == DIK_UP || keyCode == DIK_DOWN)
		vy = 0;
}
void CJasonSideview::HandleKeyDown(DWORD dt, int keyCode)
{
}



void CJasonSideview::UpdateVelocity(DWORD dt)
{
    vy += JASONSIDEVIEW_GRAVITY * dt;
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



    CAnimatableObject::Update(dt, coObjs);
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
    __instance->SetPosition(x, y);
    __instance->currentSectionId = sectionId;

    return __instance;
}


