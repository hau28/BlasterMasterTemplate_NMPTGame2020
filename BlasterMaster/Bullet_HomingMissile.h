#pragma once
#include "Bullet.h"
#include "Timer.h"

class CBullet_HomingMissile : public CBullet, public ITimeTrackable
{
private:
	const int HOMING_MISSILE_BOUNDBOX_WIDTH = 4;
	const int HOMING_MISSILE_BOUNDBOX_HEIGHT = 4;
	const int HOMING_MISSILE_BOUNDBOX_OFFSETX = 8;
	const int HOMING_MISSILE_BOUNDBOX_OFFSETY = 8;

	const float HOMING_MISSILE_ACCELERATION = 0.0008f;
	const float HOMING_MISSILE_MAX_SPEED = 0.2f;
	const float	HOMING_MISSILE_FRICTION = 0.001f;

	const int HOMING_MISSILE_DAMAGE = 8;

	bool isActivated = true;

	void UpdateFlyState();
	void ChaseTarget(DWORD dt);
	void SetDeactivatedState();

	LPGAMEOBJECT target = nullptr;

	/// <summary>
	/// check if the target still exist in the list of interactable objects.
	/// set the "target" to nullptr if not found.
	/// Call this at the beginning of Update function. please :)
	/// </summary>
	void RecheckTarget(vector<LPGAMEOBJECT>* coObjs);

	const int TIME_TO_EXPLODE = 3000;
	CTimer* explodeTimer = nullptr;
	
public:
	static const int MAX_HOMING_MISSILE_PER_USE = 3;

	CBullet_HomingMissile() {};
	CBullet_HomingMissile(float x, float y, int section, LPGAMEOBJECT target);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void HandleTimerTick(CTimer* sender);

	virtual ~CBullet_HomingMissile();
};

