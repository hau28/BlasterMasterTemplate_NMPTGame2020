#include "Sophia.h"

CSophia::CSophia(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId)
{
	SetState(SOPHIA_STATE_WALK_RIGHT);
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
		SetState(SOPHIA_STATE_WALK_RIGHT);
	else if (IsKeyDown(DIK_LEFT))
		SetState(SOPHIA_STATE_WALK_LEFT);
}
void CSophia::HandleKeyUp(DWORD dt, int keyCode)
{

}
void CSophia::HandleKeyDown(DWORD dt, int keyCode)
{
}


void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CGameObject::Update(dt, coObjs);

	HandleKeys(dt);
}

void CSophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
