#include "Bullet_Boss.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

CBullet_Boss::CBullet_Boss(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_BOSS_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	vx = nx * BULLET_BOSS_SPEED;
	vy = ny * BULLET_BOSS_SPEED;
}

void CBullet_Boss::UpdateVelocity(DWORD dt)
{
}

void CBullet_Boss::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			CGameObjectBehaviour::RemoveObject(this);
			break;
		}
		}
	}
}

void CBullet_Boss::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}
