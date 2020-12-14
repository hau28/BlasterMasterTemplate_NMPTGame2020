#include "Mine.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"


CMine::CMine(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = MINE_HEALTHPOINT;

	SetState(MINE_STATE_LIVE);
	vy = MINE_GRAVITY;
}

void CMine::UpdateVelocity(DWORD dt)
{
	// Thy cute
	vy = MINE_GRAVITY;
}

void CMine::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			case CLASS_TILE_PORTAL:
			{
				CGameObjectBehaviour::BlockObject(dt, coEvent);
				
				break;
			}
		}
	}
}

void CMine::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// Chibi cute
}

void CMine::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + MINE_BOUNDBOX_OFFSETX;
	right = left + MINE_BOUNDBOX_WIDTH;
	top = y + MINE_BOUNDBOX_OFFSETY;
	bottom = top + MINE_BOUNDBOX_HEIGHT;
}




