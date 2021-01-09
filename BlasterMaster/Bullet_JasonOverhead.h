#pragma once
#include "Bullet.h"


class BulletJasonOverheadLine {
protected:
	const float pi = 3.14;

	float vx = 1;
	float vy = 1;
	float Deg2Rad(float deg) { return deg * pi / 180; }
public:
	float getVx() { return vx; }
	float getVy() { return vy; }
	virtual void Update(float dt);
};

class Straight : public BulletJasonOverheadLine {
private:

public:
	Straight(float& speed, int level, int dx, int dy);
};

class CBullet_JasonOverhead : public CBullet
{
private:
	/// <summary>
	/// the starting position of the bullet, we need it because the bullet auto explode after a short while.
	/// this is initialize at the first call of UpdateVelocity
	/// </summary>
	
	bool flagStartedMoving = false;
	int startX = 0;
	int startY = 0;
	const float BULLET_JASON_OVERHEAD_DISTANCE = 4 * 16;

	float speed;
	BulletJasonOverheadLine* bulletLine;
	int bulletLevel;
public:
	CBullet_JasonOverhead() {};
	CBullet_JasonOverhead(float x, float y, int sectionId, int dirX, int dirY, int level);
	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
	virtual void CalcExplosionCenterPos(float& explosionX, float& explosionY);
	virtual void UpdatePosition(DWORD dt);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
};
typedef CBullet_JasonOverhead* LPBULLET_JASONOVERHEAD;
