#include "Floater.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Bullet_Floater.h"

void CFloater::UpdateState()
{
	if (vx < 0)
		SetState(FLOATER_STATE_FLY_LEFT);
	else
		SetState(FLOATER_STATE_FLY_RIGHT);
}

CFloater::CFloater(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId) 
{
	healthPoint = FLOATER_HEALTHPOINT;

	// vx = FLOATER_FLY_SPEED;
	// vy = 0;
	float tempX = 0, tempY = 0;
	while (tempX == 0 && tempY == 0)
	{
		// why mod? make sure the numbers cannot get too big like my dick
		const int THRESHOLD = abs((int)1e4);

		// magic operation -> dirty (code by CuteTN :) )
		tempX = (rand() % THRESHOLD) * ((rand() % 2) * 2 - 1);
		tempY = (rand() % THRESHOLD) * ((rand() % 2) * 2 - 1);
		// DebugOut(L"hihihihihi %f %f\n", tempX, tempY);
	}

	CGameObjectBehaviour::NormalizeVector2(tempX, tempY, vx, vy);
	vx *= FLOATER_FLY_SPEED;
	vy *= FLOATER_FLY_SPEED;

	UpdateState();

	singleShotTimer = new CTimer(this, DELAY_BETWEEN_SHOTS, SHOT_PER_SHOOTING_PHASE);
	shootPhaseTimer = new CTimer(this, DELAY_BETWEEN_SHOOTING_PHASES);
	shootTimer = new CTimer(this, SHOOT_DURATION);

};

void CFloater::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		float oldVX = vx;
		float oldVY = vy;

		CGameObjectBehaviour::BlockObject(dt, coEvent);

		if (coEvent->nx != 0)
		{
			vx = -oldVX;
		};
		if (coEvent->ny != 0)
		{
			vy = -oldVY;
		};

		UpdateState();
	}
}


void CFloater::ShootPlayer()
{
	float dirX, dirY; // direction to the player
	CGameObjectBehaviour::CalcDirectionToPlayer(this, dirX, dirY);

	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);

	if ((x - Xplayer) * vx <= 0 && y<Yplayer) {
		if (vx < 0)
			SetState(FLOATER_STATE_SHOOT_LEFT);
		else
			SetState(FLOATER_STATE_SHOOT_RIGHT);
		animationHandlers[state]->Reset();

		CBullet_Floater* bullet = new CBullet_Floater(0, 0, 0, dirX, dirY);
		CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);
	}
}

void CFloater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	singleShotTimer->Update(dt);
	shootPhaseTimer->Update(dt);
	CEnemy::Update(dt, coObjs);
}

void CFloater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + FLOATER_BOUNDBOX_OFFSETX;
	right = left + FLOATER_BOUNDBOX_WIDTH;
	top = y + FLOATER_BOUNDBOX_OFFSETY;
	bottom = top + FLOATER_BOUNDBOX_HEIGHT;
}

void CFloater::HandleTimerTick(LPTIMER sender)
{
	if (sender == singleShotTimer)
	{
		ShootPlayer();
		shootTimer->Start();
	}
	if (sender == shootTimer) {
		UpdateState();
	}

	if (sender == shootPhaseTimer)
	{
		singleShotTimer->Start();
	}
}
