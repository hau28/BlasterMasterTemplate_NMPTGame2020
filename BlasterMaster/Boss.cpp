#include "Boss.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  


CBoss::CBoss(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	SetState(PANDA_STATE_WALK_RIGHT);
	//vyMax = PANDA_MAX_FALL_SPEED;
};

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//left = x + PANDA_BOUNDBOX_OFFSETX;
	//right = left + PANDA_BOUNDBOX_WIDTH;
	//top = y + PANDA_BOUNDBOX_OFFSETY;
	//bottom = top + PANDA_BOUNDBOX_HEIGHT;
}


void CBoss::UpdateVelocity(DWORD dt)
{

}

void CBoss::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		CGameObjectBehaviour::BlockObject(dt, coEvent);

		if (coEvent->ny < 0)
		{
			flagOnAir = false;
			if (flagTouchWall)
			{
				//vy = -PANDA_JUMP;
			}
		}

		if (coEvent->nx != 0)
		{
			flagTouchWall = true;

			if (!isGoingToPlayer)
				isGoingToPlayer = true;
		}
	}
}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
}
