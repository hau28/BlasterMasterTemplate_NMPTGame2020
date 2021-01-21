#include "Bullet_Cannon.h"

#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "Sound.h"

CBullet_Cannon::CBullet_Cannon(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_ENEMY_OVERHEAD_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	vx = nx * BULLET_CANNON_SPEED;
	vy = ny * BULLET_CANNON_SPEED;

	isHiddenByForeground = true;

	zIndex = ZINDEX_OVERHEAD_ENEMY_BULLET;
}

void CBullet_Cannon::UpdateVelocity(DWORD dt)
{
}

void CBullet_Cannon::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		Sound::getInstance()->play(SMALL_PINK_BULLET_EXPLOSION, false, 1);
		CGameObjectBehaviour::BlockObject(dt, coEvent);
		CGameObjectBehaviour::RemoveObject(this);
	}
}

void CBullet_Cannon::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CBullet_Cannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	const int OFFSETY_FROM_BASE_BOUNDBOX = -11;

	CBullet::GetBoundingBox(left, top, right, bottom);
	top += OFFSETY_FROM_BASE_BOUNDBOX;
	bottom += OFFSETY_FROM_BASE_BOUNDBOX;
}
