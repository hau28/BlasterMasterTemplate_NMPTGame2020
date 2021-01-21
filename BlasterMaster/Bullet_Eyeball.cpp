#include "Bullet_Eyeball.h"

#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "Sound.h"

CBullet_Eyeball::CBullet_Eyeball(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_ENEMY_OVERHEAD_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	vx = nx * BULLET_EYEBALL_SPEED;
	vy = ny * BULLET_EYEBALL_SPEED;

	isHiddenByForeground = true;
	zIndex = ZINDEX_OVERHEAD_ENEMY_BULLET;
}

void CBullet_Eyeball::UpdateVelocity(DWORD dt)
{
}

void CBullet_Eyeball::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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

void CBullet_Eyeball::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}
