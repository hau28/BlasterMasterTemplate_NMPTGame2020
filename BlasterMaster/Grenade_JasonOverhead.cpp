#include "GameObjectBehaviour.h"

#include "TileArea.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Grenade_JasonOverhead.h"
#include "PlayScene.h"
#include "Utils.h"


void CGrenade_JasonOverhead::Init(int dx, int dy)
{
	if (dx == 1 )
		iDir = 3;

	if (dx == -1)
		iDir = 1;

	if (dy == 1)
		iDir = 4;

	if (dy == -1)
		iDir = 2;

	explosionTimer = new CTimer(this, EXPLOSION_REMOVE_DURATION, 10000);
	explosionTimer->Reset();
	explosionTimer->Stop();

}

CGrenade_JasonOverhead::CGrenade_JasonOverhead(float x, float y, int sectionId, int dirX, int dirY, float distance) : CBullet::CBullet(CLASS_JASON_OVERHEAD_GRENADE, x, y, sectionId, true)
{
	startx = dirX;
	isHiddenByForeground = true;
	isUpdatedWhenOffScreen = true;
	
	GRENADE_JASON_OVERHEAD_DISTANCE = distance;

	Init(dirX, dirY);
}

void CGrenade_JasonOverhead::UpdateVelocity(DWORD dt)
{
	if (!flagStartedMoving)
	{
		flagStartedMoving = true;
		startX = x;
		startY = y;
	}
	else
	{
		if (boom)
		{
			vx = 0;
			vy = 0;
		}
		else
		{
			if (iDir == 1)
			{
				vx = -GRENADE_VELOCITY;
				vy = dt * GRENADE_VELOCITY_y;
			}

			if (iDir == 3)
			{
				vx = GRENADE_VELOCITY;
				vy = dt * GRENADE_VELOCITY_y;
			}

			if (iDir == 2)
			{
				vy = -GRENADE_VELOCITY;
				vx = 0;
			}

			if (iDir == 4)
			{
				vy = GRENADE_VELOCITY;
				vx = 0;
			}
		}	
	}
}

void CGrenade_JasonOverhead::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		CGameObjectBehaviour::BlockObject(dt, coEvent);
		boom = true;
		explosionTimer->Start();
	}

	if (dynamic_cast<CEnemy*>(obj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(obj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CGrenade_JasonOverhead::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CGrenade_JasonOverhead::CalcExplosionCenterPos(float& explosionX, float& explosionY)
{
	CBullet::CalcExplosionCenterPos(explosionX, explosionY);
	explosionY -= BULLET_OVERHEAD_OFFSET_FROM_SHADOW;
}

void CGrenade_JasonOverhead::UpdatePosition(DWORD dt)
{
	CBullet::UpdatePosition(dt);
	if (abs(x - startX) >= GRENADE_JASON_OVERHEAD_DISTANCE)
	{
		explosionTimer->Continue();
		if (!boom)
			x = startX + (vx > 0 ? 1 : -1) * GRENADE_JASON_OVERHEAD_DISTANCE;
		boom = true;
	}
	if (abs(y - startY) >= GRENADE_JASON_OVERHEAD_DISTANCE)
	{
		explosionTimer->Continue();

		if (!boom)
			y = startY + (vy > 0 ? 1 : -1) * GRENADE_JASON_OVERHEAD_DISTANCE;

		boom = true;
	}
}

void CGrenade_JasonOverhead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	explosionTimer->Update(dt);

	if (boom)
	{
		if (boomCount == COUNT_EXPLOSION)
		{
			Explode(CLASS_EXPLOSION_OVERHEAD);
		}
	}

	CBullet::Update(dt, coObjs);
}

void CGrenade_JasonOverhead::HandleTimerTick(LPTIMER sender)
{
	if (sender == explosionTimer)
	{
		boomCount++;

		float dx = 0, dy = 0, sx, sy;

		//small
		LPEXPLOSION explosion = new CExplosion(CLASS_SMALL_CIRCLE_EXPLOSION, x, y, this->currentSectionId);
		GetPosition(sx, sy, dx, dy);

		dx = RandomFloat(-14, 14);
		dy = RandomFloat(-28, 0);

		CGameObjectBehaviour::SetBoundingBoxCenter(explosion, sx + dx, sy + dy);
		CGameObjectBehaviour::CreateObject(explosion);

		dx = RandomFloat(-14, 14);
		dy = RandomFloat(-28, 0);

		//large 
		LPEXPLOSION explosion2 = new CExplosion(CLASS_LARGE_CIRCLE_EXPLOSION, x, y, this->currentSectionId);
		GetPosition(sx, sy, dx, dy);

		CGameObjectBehaviour::SetBoundingBoxCenter(explosion2, sx + dx, sy + dy);
		CGameObjectBehaviour::CreateObject(explosion2);

		if (flagexplode)
		{
			CGameObjectBehaviour::RemoveObject(explosion2);
			CGameObjectBehaviour::RemoveObject(explosion);
			flagexplode = false;
		}
	}

}

void CGrenade_JasonOverhead::GetPosition(float& x, float& y, float dx, float dy)
{
	CGameObjectBehaviour::CalcBoundingBoxCenter(this, x, y);
}
