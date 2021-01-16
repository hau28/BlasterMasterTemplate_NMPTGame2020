#pragma once
#include "Bullet.h"

class CBullet_JasonSideview : public CBullet
{
private:
	/// <summary>
	/// the starting position of the bullet, we need it because the bullet auto explode after a short while.
	/// this is initialize at the first call of UpdateVelocity
	/// </summary>
	int startX = 0;
	bool flagStartedMoving = false;

	const int BULLET_JASON_SIDEVIEW_DAMAGE = 1;
	const float BULLET_JASON_SIDEVIEW_SPEED = 0.18f;
	const float BULLET_JASON_SIDEVIEW_DISTANCE = 6 * 16;

	const float BULLET_JASONOVERHEAD_SPEED = 0.3;

public:
	CBullet_JasonSideview() {};
	CBullet_JasonSideview(float x, float y, int section, float dirX);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
	virtual void UpdatePosition(DWORD dt);
};

