#include "Worm.h"
#include "TileArea.h"
#include "CollisionSolver.h"

CWorm::CWorm(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId)
{
	SetState(WORM_STATE_LEFT);
	vyMax = 0.9f;
};

void CWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + WORM_BOUNDBOX_OFFSETX;
	right = left + WORM_BOUNDBOX_WIDTH;
	top = y + WORM_BOUNDBOX_OFFSETY;
	bottom = top + WORM_BOUNDBOX_HEIGHT;
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

		switch (tileArea->GetClass())
		{
		case CLASS_TILE_BLOCKABLE:
		{
			x += coEvent->timeEntry * dt * vx + coEvent->nx * 0.4f;
			y += coEvent->timeEntry * dt * vy + coEvent->ny * 0.4f;
			if (coEvent->nx != 0) vx = 0;
			if (coEvent->ny != 0) vy = 0;

			// on top of a blockable tile
			if (coEvent->ny < 0)
			{
				flagOnAir = false;
			}
			break;
		}
		}
	}
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CGameObject::Update(dt, coObjs);

	// gravity effect thingy
	vy += WORM_GRAVITY;
	//DebugOut(L"why worm? why?? %f\n", vy);

	float dx, dy;
	GetPositionDifference(dx, dy);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (!flagDead)
		CCollisionSolver::CalcPotentialCollisions(this, coObjs, coEvents, dt);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		SolveClassicalMechanics();
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!

		// Filter collision get the earliest collision that can occurs in the next frame
		// FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		// CuteTN Note: wth is 0.4f??? WHAT IS IT?
		// x += min_tx * dx + nx;
		// y += min_ty * dy + ny;

		// if (nx != 0) vx = 0;
		// if (ny != 0) vy = 0;

		// collision logic
		for (LPCOLLISIONEVENT coEvent : coEventsResult)
			HandleCollision(dt, coEvent);

	}
}
