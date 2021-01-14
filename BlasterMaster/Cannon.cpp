#include "Cannon.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

void CCannon::UpdateState()
{

}

CCannon::CCannon(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = CANNON_HEALTHPOINT;

	UpdateState();

	singleShotTimer = new CTimer(this, DELAY_BETWEEN_SHOTS, SHOT_PER_SHOOTING_PHASE);
	shootPhaseTimer = new CTimer(this, DELAY_BETWEEN_SHOOTING_PHASES);
	shootTimer = new CTimer(this, SHOOT_DURATION);

};

void CCannon::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
	}
}


void CCannon::ShootPlayer()
{

}

void CCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	singleShotTimer->Update(dt);
	shootPhaseTimer->Update(dt);
	shootTimer->Update(dt);
	CEnemy::Update(dt, coObjs);
}

void CCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + CANNON_BOUNDBOX_OFFSETX;
	right = left + CANNON_BOUNDBOX_WIDTH;
	top = y + CANNON_BOUNDBOX_OFFSETY;
	bottom = top + CANNON_BOUNDBOX_HEIGHT;
}

void CCannon::HandleTimerTick(LPTIMER sender)
{
	if (sender == singleShotTimer)
	{
	}
	if (sender == shootTimer) {

	}

	if (sender == shootPhaseTimer)
	{
	}
}
