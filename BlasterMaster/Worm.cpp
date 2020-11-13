#include "Worm.h"
#include "TileArea.h"
#include "CollisionSolver.h"

CWorm::CWorm(int classId, int x, int y, int animsId) : CEnemy::CEnemy(classId, x, y, animsId)
{
	SetState(WORM_STATE_LEFT);
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

		switch (tileArea->GetClass())
		{
		case CLASS_TILE_BLOCKABLE:
		{
			// move the object to the collision position, then set the velocity to 0
			if (coEvent->nx != 0)
			{
				float dx = coEvent->rdx * coEvent->timeEntry / dt;
				this->x += dx;
				this->vx = 0;
			}

			if (coEvent->ny != 0)
			{
				float dy = coEvent->rdy * coEvent->timeEntry / dt;
				this->y += dy;
				this->vy = 0;
			}
		}
		}
	}
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CAnimatableObject::Update(dt, coObjs);
}
