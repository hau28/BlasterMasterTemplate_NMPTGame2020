#include "Teleporter.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

void CTeleporter::UpdateState()
{

}

CTeleporter::CTeleporter(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = TELEPORTER_HEALTHPOINT;

	UpdateState();

	singleShotTimer = new CTimer(this, DELAY_BETWEEN_SHOTS, SHOT_PER_SHOOTING_PHASE);
	shootPhaseTimer = new CTimer(this, DELAY_BETWEEN_SHOOTING_PHASES);
	shootTimer = new CTimer(this, SHOOT_DURATION);

};

void CTeleporter::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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


void CTeleporter::ShootPlayer()
{

}

void CTeleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	singleShotTimer->Update(dt);
	shootPhaseTimer->Update(dt);
	shootTimer->Update(dt);
	CEnemy::Update(dt, coObjs);
}

void CTeleporter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + TELEPORTER_BOUNDBOX_OFFSETX;
	right = left + TELEPORTER_BOUNDBOX_WIDTH;
	top = y + TELEPORTER_BOUNDBOX_OFFSETY;
	bottom = top + TELEPORTER_BOUNDBOX_HEIGHT;
}

void CTeleporter::HandleTimerTick(LPTIMER sender)
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
