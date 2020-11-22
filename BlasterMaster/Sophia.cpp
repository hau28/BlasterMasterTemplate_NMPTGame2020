#include "Sophia.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include "PortalLib.h"
#include "SophiaAnimationSystem.h"


CSophia::CSophia(int classId, int x, int y)
{
	this->classId = classId;
	SetPosition(x, y);
	directionState = 1;
	gunState= wheelState = 0;
	bodyState = 2;
	vyMax = SOPHIA_MAX_FALL_SPEED;
	vxMax = SOPHIA_MAX_SPEED;

};

#pragma region key events handling

void CSophia::HandleKeys(DWORD dt)
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

void CSophia::updateWheel() {

}

void CSophia::HandleKeysHold(DWORD dt)
{
	if (IsKeyDown(DIK_RIGHT))
	{
		// SetState(SOPHIA_STATE_WALK_RIGHT);
		directionState = 1;
		ax = SOPHIA_ENGINE;
	}
	else if (IsKeyDown(DIK_LEFT))
	{
		// SetState(SOPHIA_STATE_WALK_LEFT);
		directionState = 0;
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
	if (IsKeyDown(DIK_X) && ground == y)
	{
		vy = -SOPHIA_JUMP_FORCE;
	}

}
void CSophia::HandleKeyUp(DWORD dt, int keyCode)
{
	if (keyCode == DIK_RIGHT) {
		// SetState(SOPHIA_STATE_IDLE1_RIGHT);
		ax = -FRICTION;
	}
	if (keyCode == DIK_LEFT) {
		// SetState(SOPHIA_STATE_IDLE1_LEFT);
		ax = FRICTION;
	}

	if (keyCode == DIK_UP || keyCode == DIK_DOWN)
		vy = 0;
	if (keyCode == DIK_UP) {

	}
	if (vy<0 && ground-y>48.5 && keyCode == DIK_X)
		vy = 0;
}

void CSophia::HandleKeyDown(DWORD dt, int keyCode)
{
	/*	
	if (!flagOnAir && keyCode == DIK_X)
	{
		vy -= SOPHIA_JUMP_FORCE;
	}
	*/
}
#pragma endregion

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	HandleKeys(dt);
	UpdateVelocity(dt);
	flagOnAir = true;
	Deoverlap(coObjs);
	vector<LPCOLLISIONEVENT>* colEvents = new vector<LPCOLLISIONEVENT>();
	colEvents->clear();
	CheckCollision(dt, coObjs, *colEvents);
	HandleCollisions(dt, colEvents);

	UpdatePosition(dt);
	if (!flagOnAir)
		ground = y;
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
	if (directionState) {
		if (ax < 0) {
			if (flagOnAir)
				ax = -FRICTION/7;
			else
				ax = -FRICTION;
			if (vx <= 0) {
				vx = 0;
				ax = 0;
			}
		}
	}
	if (!directionState) {
		if (ax > 0) {
			if (flagOnAir)
				ax = FRICTION / 5;
			else
				ax = FRICTION;
			if (vx >= 0) {
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
			// to do: create an event to CGame, let CGame handle switching section
			DebugOut(L"To portal %d of section %d\n", toPortal->associatedPortalId, toPortal->currentSectionId);
		}
		}
	}
}
void CSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + SOPHIA_BOUNDBOX_OFFSETX;
	top = y + SOPHIA_BOUNDBOX_OFFSETY;
	right = left + SOPHIA_BOUNDBOX_WIDTH;
	bottom = top + SOPHIA_BOUNDBOX_HEIGHT;
}

void CSophia::Render()
{
	LPSPRITE sprite = CSophiaAnimationSystem::GetInstance()->GetSprite(directionState, gunState, bodyState, wheelState);
	if(sprite)
		sprite->Draw(x, y);
}

CSophia * CSophia::__instance = nullptr;

CSophia* CSophia::GetInstance()
{
	if (__instance == nullptr)
	{
		__instance = new CSophia();

	}
	return __instance;
}

CSophia* CSophia::InitInstance(int classId, int x, int y, int sectionId)
{
	delete __instance;
	__instance = new CSophia(classId, x, y);
	__instance->currentSectionId = sectionId;

	return __instance;
}


