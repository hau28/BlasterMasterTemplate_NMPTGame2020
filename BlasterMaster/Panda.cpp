#include "Panda.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  

CPanda::CPanda(int classId, int x, int y, int animsId) : CEnemy::CEnemy(classId, x, y, animsId)
{
	SetState(PANDA_STATE_WALK_RIGHT);
	vyMax = PANDA_MAX_FALL_SPEED;
};

void CPanda::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + PANDA_BOUNDBOX_OFFSETX;
	right = left + PANDA_BOUNDBOX_WIDTH;
	top = y + PANDA_BOUNDBOX_OFFSETY;
	bottom = top + PANDA_BOUNDBOX_HEIGHT;
}


void CPanda::UpdateVelocity(DWORD dt)
{
	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);

	if (isGoingToPlayer && !flagOnAir)
	{
		if (Xplayer > x)
			vx = PANDA_MOVE_SPEED;
		else
		if (Xplayer < x)
			vx = -PANDA_MOVE_SPEED;
	}

	vy += PANDA_GRAVITY;
	vy = min(vy, PANDA_MAX_FALL_SPEED);
}

void CPanda::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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

void CPanda::checkDeoverlapPlayer()
{
	float boundPlayerLeft, boundPlayerTop, boundPlayerRight, boundPlayerBottom;
	float boundPandaLeft, boundPandaTop, boundPandaRight, boundPandaBottom;
	CGame::GetInstance()->GetCurrentPlayer()->GetBoundingBox(boundPlayerLeft, boundPlayerTop, boundPandaRight, boundPandaBottom);
	this->GetBoundingBox(boundPandaLeft,boundPandaTop,boundPandaRight,boundPandaBottom);

	//if (!(boundPandaRight < boundPlayerLeft || boundPlayerRight < boundPandaLeft))
	//{
	//	int direct = rand() % 2;
	//	float 
	//}

}

void CPanda::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CAnimatableObject::Update(dt, coObjs);
}
