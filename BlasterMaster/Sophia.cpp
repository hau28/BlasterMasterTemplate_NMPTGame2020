#include "Sophia.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"



CSophia::CSophia(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId)
{
	SetState(SOPHIA_STATE_IDLE1_RIGHT);
	vyMax = 100;
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

void CSophia::HandleKeysHold(DWORD dt)
{
	if (IsKeyDown(DIK_RIGHT))
	{
		SetState(SOPHIA_STATE_WALK_RIGHT);
		vx = SOPHIA_MAX_SPEED;
		isLeft = false;
	}
	else if (IsKeyDown(DIK_LEFT))
	{
		SetState(SOPHIA_STATE_WALK_LEFT);
		vx = -SOPHIA_MAX_SPEED;
		isLeft = true;
	}
	if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_UP) && IsKeyDown(DIK_RIGHT))
	{
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
	}
	if (IsKeyDown(DIK_DOWN))
	{
	}
}
void CSophia::HandleKeyUp(DWORD dt, int keyCode)
{
	if (keyCode == DIK_RIGHT) {
		SetState(SOPHIA_STATE_IDLE1_RIGHT);
		flagStop = true;
		stopLeft = false;

		vx = 0;
	}
	if (keyCode == DIK_LEFT) {
		SetState(SOPHIA_STATE_IDLE1_LEFT);
		flagStop = true;
		stopLeft = true;

		vx = 0;
	}

	if (keyCode == DIK_UP || keyCode == DIK_DOWN)
		vy = 0;
	if (keyCode == DIK_UP) {
		if (!isLeft) {
			SetState(SOPHIA_STATE_GUNDOWN_RIGHT);
			animationHandlers[state]->currentFrameIndex = 0;
		}
		if (isLeft) {
			SetState(SOPHIA_STATE_GUNDOWN_LEFT);
			animationHandlers[state]->currentFrameIndex = 0;
		}
	}
}

void CSophia::HandleKeyDown(DWORD dt, int keyCode)
{
	if (!flagOnAir && keyCode == DIK_X)
	{
		vy -= SOPHIA_JUMP_FORCE;
	}
}

void CSophia::UpdateVelocity(DWORD dt)
{
	// CuteTN Todo: Test
	vy += SOPHIA_GRAVITY;
	vy = min(vy, SOPHIA_MAX_FALL_SPEED);
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

		switch (tileArea->GetClass())
		{
		case CLASS_TILE_BLOCKABLE:
		{
			CGameObjectBehaviour::GetBlocked(dt, coEvent);

			if (coEvent->ny < 0)
				flagOnAir = false;

			break;
		}
		}
	}
}

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	HandleKeys(dt);
	flagOnAir = true;
	CAnimatableObject::Update(dt, coObjs);
}

void CSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + SOPHIA_BOUNDBOX_OFFSETX;
	top = y + SOPHIA_BOUNDBOX_OFFSETY;
	right = left + SOPHIA_BOUNDBOX_WIDTH;
	bottom = top + SOPHIA_BOUNDBOX_HEIGHT;
}

CSophia * CSophia::__instance = nullptr;

CSophia* CSophia::GetInstance()
{
	if (__instance == nullptr)
		__instance = new CSophia();
	return __instance;
}

CSophia* CSophia::InitInstance(int classId, int x, int y, int animsId)
{
	delete __instance;
	__instance = new CSophia(classId, x, y, animsId);

	return __instance;
}


