#pragma once
#include "Bullet.h"
class CBullet_ThunderBreak : public CBullet
{
private:
	const int THUNDERBREAK_CHAIN_LENGTH = 8;
	CBullet_ThunderBreak(float x, float y, int section, int indexInChain);

	const int THUNDERBREAK_LIFETIME = 1000;
	const int THUNDERBREAK_DELAY_PER_UNIT = 100;

	void SetRandomState();
	void CreateNextUnit();

public:

	CBullet_ThunderBreak() {};
	CBullet_ThunderBreak(float x, float y, int section);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

