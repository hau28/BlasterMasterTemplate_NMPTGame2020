#include "Bullet_JasonOverhead.h"
#include "GameObjectBehaviour.h"

#include "TileArea.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"


CBullet_JasonOverhead::CBullet_JasonOverhead(float x, float y, int sectionId, int dirX, int dirY, int level) : CBullet::CBullet(CLASS_JASON_OVERHEAD_BULLET, x, y, sectionId, false)
{
	bulletLevel = level;
	if (bulletLevel <= 4)
	{ // bullets 1 -> 4
		bulletLine = new Straight(speed, bulletLevel, dirX, dirY);
	}
}

void CBullet_JasonOverhead::UpdateVelocity(DWORD dt)
{
	if (!flagStartedMoving)
	{
		flagStartedMoving = true;
		startX = x;
		startY = y;
	}

	bulletLine->Update(dt);
	vx = speed * bulletLine->getVx();
	vy = speed * bulletLine->getVy();
}

void CBullet_JasonOverhead::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			Explode(CLASS_EXPLOSION_OVERHEAD);
			break;
		}
		}
	}

	if (dynamic_cast<CEnemy*>(obj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(obj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_JasonOverhead::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_JasonOverhead::CalcExplosionCenterPos(float& explosionX, float& explosionY)
{
}

void CBullet_JasonOverhead::UpdatePosition(DWORD dt)
{
	CBullet::UpdatePosition(dt);
	if (bulletLevel == 0)
	{
		if (abs(x - startX) >= BULLET_JASON_OVERHEAD_DISTANCE)
		{
			x = startX + (vx > 0 ? 1 : -1) * BULLET_JASON_OVERHEAD_DISTANCE;
			CGameObjectBehaviour::RemoveObject(this);
		}
		if (abs(y - startY) >= BULLET_JASON_OVERHEAD_DISTANCE) 
		{
			y = startX + (vx > 0 ? 1 : -1) * BULLET_JASON_OVERHEAD_DISTANCE;
			CGameObjectBehaviour::RemoveObject(this);
		}
	}

}

void CBullet_JasonOverhead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CBullet::Update(dt, coObjs);
}

void BulletJasonOverheadLine::Update(float dt)
{
	//thy cute	

}

Straight::Straight(float& speed, int level, int dx, int dy) {
	speed = 0.2;
	//livingTime = BASE_LIVINGTIME * (level >= 2 ? 99999 : 1); //all bullets which its level is higher than 2 is not disapear automatically

	if (dx == 0) vx = 0;
	else vx = (dx < 0 ? -1 : 1);

	if (dy == 0) vy = 0;
	else vy = (dy < 0 ? -1 : 1);
}