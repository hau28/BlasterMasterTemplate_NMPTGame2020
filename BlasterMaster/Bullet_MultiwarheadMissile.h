#pragma once
#include "Bullet.h"
class CBullet_MultiwarheadMissile : public CBullet
{
private:
	const int MULTIWARHEAD_MISSILE_BOUNDBOX_OFFSETX = 4;
	const int MULTIWARHEAD_MISSILE_BOUNDBOX_OFFSETY = 4;
	const int MULTIWARHEAD_MISSILE_BOUNDBOX_WIDTH = 8;
	const int MULTIWARHEAD_MISSILE_BOUNDBOX_HEIGHT = 8;

	const int MULTIWARHEAD_MISSILE_DAMAGE = 10;

	const float MULTIWARHEAD_MISSILE_ACCELERATION_X = 0.0005f;
	const float MULTIWARHEAD_MISSILE_MAXVELOCITY_X = 0.4f;

public:
	static const float MULTIWARHEAD_MISSILE_INIT_VELOCITY_Y_TOP;

	CBullet_MultiwarheadMissile() {};
	CBullet_MultiwarheadMissile::CBullet_MultiwarheadMissile(float x, float y, int section, bool initLeft, float initVx, float initVy);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

