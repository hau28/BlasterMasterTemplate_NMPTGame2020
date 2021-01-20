#include "Bullet_JasonOverhead.h"
#include "GameObjectBehaviour.h"

#include "TileArea.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"


CBullet_JasonOverhead::CBullet_JasonOverhead(float x, float y, int sectionId, int dirX, int dirY, int level, int index) : CBullet::CBullet(CLASS_JASON_OVERHEAD_BULLET, x, y, sectionId, true)
{
	startx = dirX;
	this->bulletLevel = level;
	if (bulletLevel < 4) // bullets 0-> 3
		bulletLine = new StraightLine(speed, bulletLevel, dirX, dirY);

	if (bulletLevel == 4 || bulletLevel == 5)
	{
		bulletLine = new CircleLine(speed, bulletLevel, dirX, dirY);
	}
	
	if (bulletLevel > 5)
		bulletLine = new WaveLine(speed, bulletLevel, dirX, dirY);

	isHiddenByForeground = true;
	isUpdatedWhenOffScreen = true;
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
	vx = speed*2 * bulletLine->getVx();
	vy = speed*2 * bulletLine->getVy();
	
}

void CBullet_JasonOverhead::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		//if (coEvent->nx != 0)
		//{
		CGameObjectBehaviour::BlockObject(dt, coEvent);
		Explode(CLASS_EXPLOSION_OVERHEAD);
		//}
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
	CBullet::CalcExplosionCenterPos(explosionX, explosionY);
	explosionY -= BULLET_OVERHEAD_OFFSET_FROM_SHADOW;
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
			// Explode(CLASS_EXPLOSION_OVERHEAD);
		}
		if (abs(y - startY) >= BULLET_JASON_OVERHEAD_DISTANCE)
		{
			y = startY + (vy > 0 ? 1 : -1) * BULLET_JASON_OVERHEAD_DISTANCE;
			CGameObjectBehaviour::RemoveObject(this);
			Explode(CLASS_EXPLOSION_OVERHEAD);
		}
	}

}

void CBullet_JasonOverhead::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (startx == x)
	{
		flagOver = true;
	}
	CBullet::Update(dt, coObjs);
}

void BulletJasonOverheadLine::Update(DWORD dt)
{
	//thy cute	
}

StraightLine::StraightLine(float& speed, int level, int dx, int dy) {
	speed = BULLET_JASONOVERHEAD_SPEED;

	if (dx == 0) vx = 0;
	else vx = (dx < 0 ? -1 : 1);

	if (dy == 0) vy = 0;
	else vy = (dy < 0 ? -1 : 1);
}

CircleLine::CircleLine(float& speed, int level, int dx, int dy)
{
	this->speed = speed = BULLET_JASONOVERHEAD_SPEED * (level == 5 ? 1 : 0.66);
	this->radius = level == 4 ? BULLET_JASONOVERHEAD_SMALL_CIRCLE_RADIUS : BULLET_JASONOVERHEAD_LARGE_CIRCLE_RADIUS;
	
	if (dx == 0)
	{
		vx = 0;
		vy = (dy == -1 ? -1 : 1);
	}
	else
	{
		vy = 0;
		vx = (dx == -1 ? -1 : 1);
	}

	dir = arrDir[iDir];
	iDir = (iDir + 1) % 4;

	// CuteTN
	this->angularVelocity = speed / radius;
}

int CircleLine::iDir = 0;
int CircleLine::arrDir[4] = { 0,1,0,-1 };

void CircleLine::Update(DWORD dt)
{
	// float alpha = 2* pi * dt * dir;
	float alpha = angularVelocity * dt * dir;

	float newVX = vx * cos(alpha) - vy * sin(alpha);
	float newVY = vy * cos(alpha) + vx * sin(alpha);

	vx = newVX;
	vy = newVY;

	sumAngle += alpha;
	int curRoundCount = (int)(abs(sumAngle) / (2 * pi));
	if (curRoundCount > roundCount)
	{
		radius += (curRoundCount - roundCount) * BULLET_JASONOVERHEAD_INCREASING_CIRCLE_RADIUS;
		angularVelocity = speed / radius;
		roundCount = curRoundCount;
	}
}

void WaveLine::Init(int dx, int dy)
{
	if (dx == 1)
		direction = 3;

	if (dx == -1)
		direction = 1;

	if (dy == 1)
		direction = 4;

	if (dy == -1)
		direction = 2;
}

WaveLine::WaveLine(float& speed, int level, int dx, int dy)
{
	//speed = (level == 6 ? BULLET_JASONOVERHEAD_SPEED * 0.5 : BULLET_JASONOVERHEAD_SPEED*0.7);
	speed = BULLET_JASONOVERHEAD_SPEED;
	Init(dx, dy);
	
	originalAngle = arrAngle[iAngle];
	iAngle = (iAngle + 1) % 2;

	velocity = (level == 6 ? BULLET_JASONOVERHEAD_SPEED : BULLET_JASONOVERHEAD_SPEED * 2);
}

int WaveLine::iAngle = 0;
int WaveLine::arrAngle[2] = { 60,220 };

void WaveLine::Update(DWORD dt)
{
	updateTime += 50;
	omega -= 0.05;
	if (direction == 4)
	{
		vx = AMPLITUDE * cos(updateTime + originalAngle);
		vy = velocity;
	}

	if (direction == 2)
	{
		vx = AMPLITUDE * cos(updateTime + originalAngle);
		vy = -velocity;
	}

	if (direction == 3)
	{
		vy = AMPLITUDE * cos(updateTime + originalAngle);
		vx = velocity;
	}

	if (direction == 1)
	{
		vy = AMPLITUDE * cos(updateTime + originalAngle);
		vx = -velocity;
	}

}

