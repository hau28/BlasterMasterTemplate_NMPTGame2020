#include "Bullet_MultiwarheadMissile.h"
#include "Enemy.h"
#include "GameObjectBehaviour.h"
#include "Sound.h"

const float CBullet_MultiwarheadMissile::MULTIWARHEAD_MISSILE_INIT_VELOCITY_Y_TOP = -0.065f;

CBullet_MultiwarheadMissile::CBullet_MultiwarheadMissile(float x, float y, int section, bool initLeft, float initVx, float initVy) : CBullet(CLASS_MULTIWARHEAD_MISSILE, x, y, section, true)
{
	Sound::getInstance()->play(MISSILE, false, 1);
	SetState(initLeft ? MULTIWARHEAD_MISSILE_STATE_FLY_LEFT : MULTIWARHEAD_MISSILE_STATE_FLY_RIGHT);

	vx = initVx;
	vy = initVy;

	ax = MULTIWARHEAD_MISSILE_ACCELERATION_X;
	if (initLeft)
		ax *= -1;
	
	this->isHiddenByForeground = false;
	this->allowOverlapWithBlocks = true;
	this->isUpdatedWhenOffScreen = true;

	this->damage = MULTIWARHEAD_MISSILE_DAMAGE;
}


void CBullet_MultiwarheadMissile::UpdateVelocity(DWORD dt)
{
	vx += ax * dt;
	vx = min(vx,  MULTIWARHEAD_MISSILE_MAXVELOCITY_X);
	vx = max(vx, -MULTIWARHEAD_MISSILE_MAXVELOCITY_X);
}

void CBullet_MultiwarheadMissile::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (dynamic_cast<CEnemy*>(obj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(obj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_MultiwarheadMissile::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_MultiwarheadMissile::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// CuteTN todo: rotate bounding box maybe
	// CuteTN this is hardcoded just for testing purpose, please do NOT follow :)
	left = x + MULTIWARHEAD_MISSILE_BOUNDBOX_OFFSETX;
	top = y + MULTIWARHEAD_MISSILE_BOUNDBOX_OFFSETY;
	right = left + MULTIWARHEAD_MISSILE_BOUNDBOX_WIDTH;
	bottom = top + MULTIWARHEAD_MISSILE_BOUNDBOX_HEIGHT;
}
