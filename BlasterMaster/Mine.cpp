#include "Mine.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"
#include "Sophia.h"
#include "JasonSideview.h"
#include "Bullet_Mine.h"
#include "Utils.h"

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

	if (CCollisionSolver::IsOverlapped(this, CGame::GetInstance()->GetCurrentPlayer()))
	{
		CGameObjectBehaviour::RemoveObject(this);// delete enemy

		float dirX, dirY;
		CGameObjectBehaviour::CalcDirecttionToPlayer(this, dirX, dirY);

		int numberOfBullet = rand() % 5;	

		for (int i = 0; i < numberOfBullet; i++) 
		{
			float x_factor = RandomFloat(-3, 3);
			float y_factor = RandomFloat(0, 5);

			CBullet_Mine* bullet = new CBullet_Mine(0,0, 0, dirX + x_factor, dirY + y_factor);
			CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);
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




