#include "Bullet_Mine.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"
#include "GameObject.h"

CBullet_Mine::CBullet_Mine(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_LARGE_PINK_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	float vx_factor = RandomFloat(-1, 1);
	float vy_factor = RandomFloat(0.1, 1);

	vx = BASEVELOCITY*vx_factor;
	vy = -BASEVELOCITY*vy_factor;

	this->isUpdatedWhenOffScreen = false;
	this->allowOverlapWithBlocks = true;

	timestartDrop = GetTickCount();
}

void CBullet_Mine::UpdateVelocity(DWORD dt)
{
	vy += BASEVELOCITY*0.1;

	if ( GetTickCount() - timestartDrop > 200)
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
	if (!checkObjInCamera(this, SCREEN_EXTEND_OFFSET_DEFAULT)) // chỗ này cũng lười viết hàm update nên t bỏ đây luôn
		CGameObjectBehaviour::RemoveObject(this);
}

void CBullet_Mine::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}
