#include "Bullet_JasonSideview.h"

#include "TileArea.h"
#include "GameObjectBehaviour.h"

CBullet_JasonSideview::CBullet_JasonSideview(float x, float y, int sectionId, float dirX) : CBullet::CBullet(CLASS_SMALL_PINK_BULLET, x, y, sectionId, true)
{
	damage = BULLET_JASON_SIDEVIEW_DAMAGE;

	dirX = dirX > 0 ? 1 : -1;

	vx = dirX * BULLET_JASON_SIDEVIEW_SPEED;
	vy = 0;
}

void CBullet_JasonSideview::UpdateVelocity(DWORD dt)
{
	if (!flagStartedMoving)
	{
		flagStartedMoving = true;
		startX = x;
	}
}

void CBullet_JasonSideview::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		CGameObjectBehaviour::BlockObject(dt, coEvent);
		Explode(CLASS_SMALL_EXPLOSION_SIDEVIEW);
	}

	if (dynamic_cast<CEnemy*>(obj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(obj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_JasonSideview::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_JasonSideview::UpdatePosition(DWORD dt)
{
	CBullet::UpdatePosition(dt);

	if (abs(x - startX) >= BULLET_JASON_SIDEVIEW_DISTANCE)
	{
		x = startX + (vx > 0 ? 1 : -1) * BULLET_JASON_SIDEVIEW_DISTANCE;
		Explode(CLASS_SMALL_EXPLOSION_SIDEVIEW);
	}
}

