#pragma once
#include "Bullet.h"
#include "Timer.h"

class CGrenade_JasonOverhead :public CBullet, public ITimeTrackable
{
private:
	/// <summary>
	/// the starting position of the bullet, we need it because the bullet auto explode after a short while.
	/// this is initialize at the first call of UpdateVelocity
	/// </summary>
	
	float boundingBoxOffsetX = 8;
	float boundingBoxOffsetY = 8;
	float boundingBoxWidth = 21;
	float boundingBoxHeight = 3;

	bool flagStartedMoving = false;
	float startX = 0;
	float startY = 0;
	int startx;
	/*const float GRENADE_JASON_OVERHEAD_DISTANCE = 2.5 * 16;*/
	const float GRENADE_VELOCITY = 0.3;
	const float GRENADE_VELOCITY_y = 0.002;
	const float GRENADE_JASON_OVERHEAD_DAMAGE = 2;

	float speed;
	int bulletLevel;

	bool flagOver = false;

	void Init(int dx, int dy);
	int iDir;

	bool boom = false;
	int boomCount = 0;

	LPTIMER explosionTimer;
	LPTIMER reloadTimer;

	const int EXPLOSION_REMOVE_DURATION = 40;

	const int COUNT_EXPLOSION = 5;

	const int Thy = 5;
	bool flagexplode = false;
	bool flagreload = false;

	float GRENADE_JASON_OVERHEAD_DISTANCE;
public:
	CGrenade_JasonOverhead() {};
	CGrenade_JasonOverhead(float x, float y, int sectionId, int dirX, int dirY, float distance);
	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
	virtual void CalcExplosionCenterPos(float& explosionX, float& explosionY);
	virtual void UpdatePosition(DWORD dt);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
	virtual void HandleTimerTick(LPTIMER sender);
	bool getflag() { return flagOver; };

	void GetPosition(float& x, float& y, float dx, float dy);
	//virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};
typedef CGrenade_JasonOverhead* LPGRENADEJASONOVERHEAD;

