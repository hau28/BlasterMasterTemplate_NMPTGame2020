#pragma once
#include "Bullet.h"
#include "Timer.h"

class CBullet_ThunderBreak : public CBullet, public ITimeTrackable
{
private:
	const int THUNDERBREAK_BOUNDBOX_OFFSETX = 0;
	const int THUNDERBREAK_BOUNDBOX_OFFSETY = 0;
	const int THUNDERBREAK_BOUNDBOX_WIDTH = 16;
	const int THUNDERBREAK_BOUNDBOX_HEIGHT = 16;

	const int THUNDERBREAK_OFFSETX_UNITS = 6;

	const int THUNDERBREAK_DAMAGE = 10;

	const int THUNDERBREAK_CHAIN_LENGTH = 8;
	int indexInChain = 0;
	CBullet_ThunderBreak(float x, float y, int section, int indexInChain);

	const int THUNDERBREAK_LIFETIME = 100;
	const int THUNDERBREAK_DELAY_PER_UNIT = 10;

	void SetRandomState();
	void CreateNextUnit();

	LPTIMER lifetimeTimer = nullptr;
	LPTIMER delayToNextUnitTimer = nullptr;

public:
	CBullet_ThunderBreak() {};
	CBullet_ThunderBreak(float x, float y, int section);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	virtual void HandleTimerTick(CTimer* sender);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

