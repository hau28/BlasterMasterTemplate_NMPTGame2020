#pragma once
#include "Bullet.h"
class CBullet_Boss :public CBullet
{
public :
	const float BULLET_BOSS_SPEED = 0.1f;

	CBullet_Boss() {};
	CBullet_Boss(float x, float y, int section, float dirX, float dirY);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
};

