#include "Worm.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"


CWorm::CWorm(int classId, int x, int y, int animsId) : CEnemy::CEnemy(classId, x, y, animsId)
{
	SetState(WORM_STATE_RIGHT);
	vyMax = WORM_MAX_FALL_SPEED;
};

void CWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + WORM_BOUNDBOX_OFFSETX;
	right = left + WORM_BOUNDBOX_WIDTH;
	top = y + WORM_BOUNDBOX_OFFSETY;
	bottom = top + WORM_BOUNDBOX_HEIGHT;
}


void CWorm::UpdateVelocity(DWORD dt)
{
	// CuteTN Todo: Test
	vx = WORM_MOVE_SPEED;
	
	vy += WORM_GRAVITY;
	vy = min(vy, WORM_MAX_FALL_SPEED);
}

void CWorm::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			break;
		}
		}
	}
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CAnimatableObject::Update(dt, coObjs);
}
