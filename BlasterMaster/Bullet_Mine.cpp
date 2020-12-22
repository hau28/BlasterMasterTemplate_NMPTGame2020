#include "Bullet_Mine.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"
#include "GameObject.h"

CBullet_Mine::CBullet_Mine(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_LARGE_PINK_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	//float vx_factor = RandomFloat(-1, 1);
	//float vy_factor = RandomFloat(0.1, 1);

	vx = BASEVELOCITY*nx;
	vy = BASEVELOCITY*ny;

	this->isUpdatedWhenOffScreen = true;
	this->allowOverlapWithBlocks = true;

	timestartDrop = GetTickCount();
}

void CBullet_Mine::UpdateVelocity(DWORD dt)
{
	vy += BASEVELOCITY*0.1;

	if ( GetTickCount() - timestartDrop > 300)
	{
		if (CCollisionSolver::IsOverlapped(this, CGame::GetInstance()->GetCurrentPlayer()))
		{
			CGameObjectBehaviour::RemoveObject(this);
			Explode(CLASS_LARGE_EXPLOSION_SIDEVIEW);
		}
	}
}

void CBullet_Mine::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CBullet_Mine::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}
