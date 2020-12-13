#include "Bullet_Floater.h"

#include "TileArea.h"
#include "GameObjectBehaviour.h"

CBullet_Floater::CBullet_Floater(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_SMALL_PINK_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	vx = nx * BULLET_FLOATER_SPEED;
	vy = ny * BULLET_FLOATER_SPEED;
}

void CBullet_Floater::UpdateVelocity(DWORD dt)
{
}

void CBullet_Floater::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			Explode(CLASS_SMALL_EXPLOSION_SIDEVIEW);
			break;
		}
		}
	}
}

void CBullet_Floater::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}
