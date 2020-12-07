#include "Skull.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"


CSkull::CSkull(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	SetState(SKULL_STATE_FLY_LEFT);
}

void CSkull::UpdateVelocity(DWORD dt)
{
	// Thy cute
}

void CSkull::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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

void CSkull::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// Chibi cute
}

void CSkull::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + SKULL_BOUNDBOX_OFFSETX;
	right = left + SKULL_BOUNDBOX_WIDTH;
	top = y + SKULL_BOUNDBOX_OFFSETY;
	bottom = top + SKULL_BOUNDBOX_HEIGHT;
}




