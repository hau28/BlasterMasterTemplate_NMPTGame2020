#include "Bullet_HomingMissile.h"
#include "GameObjectBehaviour.h"


CBullet_HomingMissile::CBullet_HomingMissile(float x, float y, int section, LPGAMEOBJECT target) : CBullet(CLASS_HOMING_MISSILE, x, y, section, true)
{
	this->target = target;

	this->isHiddenByForeground = false;
	this->allowOverlapWithBlocks = true;
	this->isUpdatedWhenOffScreen = true;

	this->damage = HOMING_MISSILE_DAMAGE;

	explodeTimer = new CTimer(this, TIME_TO_EXPLODE, 1);

	UpdateFlyState();
}

void CBullet_HomingMissile::RecheckTarget(vector<LPGAMEOBJECT>* coObjs)
{
	for (auto obj : *coObjs)
	{
		if (obj == target && obj)
			return;
	}

	target = nullptr;
}

void CBullet_HomingMissile::UpdateFlyState()
{
	int newState = state;

	if (abs(vx) > abs(vy))
	{
		newState = vx < 0 ? HOMING_MISSILE_STATE_FLY_LEFT : HOMING_MISSILE_STATE_FLY_RIGHT;
	}
	else
	{
		newState = vy < 0 ? HOMING_MISSILE_STATE_FLY_UP : HOMING_MISSILE_STATE_FLY_DOWN;
	}

	SetState(newState);
}

void CBullet_HomingMissile::ChaseTarget(DWORD dt)
{
	if (!target)
		return;

	float dirX, dirY;

	CGameObjectBehaviour::CalcDirection(this, target, dirX, dirY);
	
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);
	dirX = nx * HOMING_MISSILE_ACCELERATION;
	dirY = ny * HOMING_MISSILE_ACCELERATION;

	vx += dirX * dt;
	vy += dirY * dt;

	vx *= (1 - HOMING_MISSILE_FRICTION);
	vy *= (1 - HOMING_MISSILE_FRICTION);

	// make sure velocity does not exceed max velocity
	float scaleDownRatio = HOMING_MISSILE_MAX_SPEED / CGameObjectBehaviour::CalcMagnitudeVector2(vx, vy);
	if (scaleDownRatio < 1)
	{
		vx *= scaleDownRatio;
		vy *= scaleDownRatio;
	}
}

void CBullet_HomingMissile::SetDeactivatedState()
{
	switch (state)
	{
	case HOMING_MISSILE_STATE_FLY_LEFT:
		SetState(HOMING_MISSILE_STATE_OFF_LEFT);
		break;

	case HOMING_MISSILE_STATE_FLY_RIGHT:
		SetState(HOMING_MISSILE_STATE_OFF_RIGHT);
		break;

	case HOMING_MISSILE_STATE_FLY_DOWN:
		SetState(HOMING_MISSILE_STATE_OFF_DOWN);
		break;

	case HOMING_MISSILE_STATE_FLY_UP:
		SetState(HOMING_MISSILE_STATE_OFF_UP);
		break;
	}
}



void CBullet_HomingMissile::UpdateVelocity(DWORD dt)
{
	if (!target && isActivated)
	{
		SetDeactivatedState();
		isActivated = false;
	}

	if (isActivated)
	{
		ChaseTarget(dt);
		UpdateFlyState();
	}
}

void CBullet_HomingMissile::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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

void CBullet_HomingMissile::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy);
	}
}

void CBullet_HomingMissile::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	explodeTimer->Update(dt);

	RecheckTarget(coObjs);

	// CuteTN Note: we don't want homing missiles to be auto destroyed when off screen
	// CBullet::Update(dt, coObjs);
	CAnimatableObject::Update(dt, coObjs);
}

void CBullet_HomingMissile::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// CuteTN todo: rotate bounding box maybe
	// CuteTN this is hardcoded just for testing purpose, please do NOT follow :)
	left = x + HOMING_MISSILE_BOUNDBOX_OFFSETX;
	top = y + HOMING_MISSILE_BOUNDBOX_OFFSETY;
	right = left + HOMING_MISSILE_BOUNDBOX_WIDTH;
	bottom = top + HOMING_MISSILE_BOUNDBOX_HEIGHT;
}

void CBullet_HomingMissile::HandleTimerTick(CTimer* sender)
{
	Explode(CLASS_SMALL_EXPLOSION_SIDEVIEW);
}

CBullet_HomingMissile::~CBullet_HomingMissile()
{
	delete explodeTimer;
	CBullet::~CBullet();
}
