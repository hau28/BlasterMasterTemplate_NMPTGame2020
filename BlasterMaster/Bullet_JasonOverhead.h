#pragma once
#include "Bullet.h"


class BulletJasonOverheadLine {
protected:
	const float pi = 3.141592654;
	const float BULLET_JASONOVERHEAD_SPEED = 0.15;
	const float BASE_LIVINGTIME = 0.3; //300m

	float vx = 1;
	float vy = 1;

	bool flagover = false;

public:
	float getVx() { return vx; }
	float getVy() { return vy; }
	virtual void Update(DWORD dt);
};

class StraightLine : public BulletJasonOverheadLine {
private:

public:
	StraightLine(float& speed, int level, int dx, int dy);
};

class CircleLine : public BulletJasonOverheadLine {
private:
	const float BULLET_JASONOVERHEAD_SMALL_CIRCLE_RADIUS = 16;
	const float BULLET_JASONOVERHEAD_LARGE_CIRCLE_RADIUS = 32;
	const float BULLET_JASONOVERHEAD_INCREASING_CIRCLE_RADIUS = 10;

	static int iDir;
	static int arrDir[4];

	int dir;
	float curAngle;

	float angularVelocity;
	float speed;
	float radius;

	float sumAngle = 0;
	int roundCount = 0;

public:
	CircleLine(float& speed, int level, int dx, int dy);
	void Update(DWORD dt);
};

class WaveLine : public BulletJasonOverheadLine {
private:

	static float nextDirection;
	//adjustment between bullets
	const float DELAY_WAVE_OFFSET = 0.025;
	const float DELAY_WAVE_MAX = 0.1;
	static float nextDelay;

	float direction;

	float delay; //if the bullet is "delaying", it flies straight

	const float BASE_FI_OFFSET = 3.14 * 2; //approximately 4*PI per second
	float fiOffset; //RAD
	float fi = 0;	//RAD

	//at lv7, lv8 is adjusted in code
	float spreadDiv = 6;
	float spread = 1.65;

	bool isHorizontal;
public:
	WaveLine(float& speed, int level, int dx, int dy, float& livingTime);
	void Update(DWORD dt);
};

class CBullet_JasonOverhead : public CBullet
{
private:
	/// <summary>
	/// the starting position of the bullet, we need it because the bullet auto explode after a short while.
	/// this is initialize at the first call of UpdateVelocity
	/// </summary>

	bool flagStartedMoving = false;
	float startX = 0;
	float startY = 0;
	int startx;
	const float BULLET_JASON_OVERHEAD_DISTANCE = 4 * 16;

	float speed;
	BulletJasonOverheadLine* bulletLine;
	int bulletLevel;
	float livingTime = 0;
	bool flagOver = false;
public:
	CBullet_JasonOverhead() {};
	CBullet_JasonOverhead(float x, float y, int sectionId, int dirX, int dirY, int level, int index);
	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
	virtual void CalcExplosionCenterPos(float& explosionX, float& explosionY);
	virtual void UpdatePosition(DWORD dt);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	bool getflag() { return flagOver; };
	
};
typedef CBullet_JasonOverhead* LPBULLET_JASONOVERHEAD;
