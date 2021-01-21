#include "Bullet_ThunderBreak.h"
#include "Enemy.h"
#include "GameObjectBehaviour.h"
#include "Sound.h"

CBullet_ThunderBreak::CBullet_ThunderBreak(float x, float y, int section, int indexInChain) : CBullet(CLASS_THUNDERBREAK, x, y, section, true)
{

	Sound::getInstance()->play(THUNDER, false, 1);
	vx = vy = ax = ay = 0;

	this->indexInChain = indexInChain;

	this->damage = THUNDERBREAK_DAMAGE;

	allowOverlapWithBlocks = true;
	isUpdatedWhenOffScreen = true;
	isHiddenByForeground = false;

	SetRandomState();

	int thisUnitLifetime = THUNDERBREAK_LIFETIME - indexInChain * THUNDERBREAK_DELAY_PER_UNIT;
	lifetimeTimer = new CTimer(this, thisUnitLifetime, 1);
	delayToNextUnitTimer = new CTimer(this, THUNDERBREAK_DELAY_PER_UNIT, 1);

	// set random bright color
	const int MIN_COLOR_R = 200;
	const int MIN_COLOR_G = 150;
	const int MIN_COLOR_B = 150;
	modifyR = MIN_COLOR_R + rand() % (256 - MIN_COLOR_R);
	modifyG = MIN_COLOR_G + rand() % (256 - MIN_COLOR_G);
	modifyB = MIN_COLOR_B + rand() % (256 - MIN_COLOR_B);
}

CBullet_ThunderBreak::CBullet_ThunderBreak(float x, float y, int section) : CBullet_ThunderBreak(x, y, section, 0)
{
}

void CBullet_ThunderBreak::SetRandomState()
{
	int x = rand() % 4;

	switch (x)
	{
	case 1: SetState(THUNDERBREAK_STATE_1); return;
	case 2: SetState(THUNDERBREAK_STATE_2); return;
	case 3: SetState(THUNDERBREAK_STATE_3); return;
	case 4: SetState(THUNDERBREAK_STATE_4); return;
	}
}

void CBullet_ThunderBreak::UpdateVelocity(DWORD dt)
{
}

void CBullet_ThunderBreak::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CBullet_ThunderBreak::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy, true);
	}
}

void CBullet_ThunderBreak::CreateNextUnit()
{
	float newX = x + THUNDERBREAK_OFFSETX_UNITS * ((rand() % 2) * 2 - 1);
	float newY = y + THUNDERBREAK_BOUNDBOX_HEIGHT;

	CBullet_ThunderBreak* nextUnit = new CBullet_ThunderBreak(newX, newY, currentSectionId, indexInChain + 1);
	CGameObjectBehaviour::CreateObject(nextUnit);
}

void CBullet_ThunderBreak::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	lifetimeTimer->Update(dt);
	delayToNextUnitTimer->Update(dt);

	CBullet::Update(dt, coObjs);
}

void CBullet_ThunderBreak::HandleTimerTick(CTimer* sender)
{
	if (sender == lifetimeTimer)
	{
		CGameObjectBehaviour::RemoveObject(this);
	}

	if (sender == delayToNextUnitTimer)
	{
		if(indexInChain < THUNDERBREAK_CHAIN_LENGTH - 1)
			CreateNextUnit();
	}
}

void CBullet_ThunderBreak::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + THUNDERBREAK_BOUNDBOX_OFFSETX;
	top = y + THUNDERBREAK_BOUNDBOX_OFFSETY;
	right = left + THUNDERBREAK_BOUNDBOX_WIDTH;
	bottom = top + THUNDERBREAK_BOUNDBOX_HEIGHT;
}

