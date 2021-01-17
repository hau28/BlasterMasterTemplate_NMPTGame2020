#include "Eyeball.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Utils.h"
#include "Bullet_Eyeball.h"

void CEyeball::UpdateState()
{
}

CEyeball::CEyeball(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = EYEBALL_HEALTHPOINT;
	UpdateState();
	flagMove = false;
	moveTimer = new CTimer(this, DELAY_BETWEEN_MOVE);
};

void CEyeball::approach() {
	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);
	flagRight = true;
	flagDown = true;
	decision = rand() % 3;
	if (decision == 0) {
		if (Xplayer >= x)
			vx = RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
		else {
			vx = -RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
			flagRight = false;
		};
		if (Yplayer >= y)
			vy = RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
		else {
			vy = -RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
			flagDown = false;
		}
	}
	else if (decision == 1) {
		if (Xplayer < x)
			vx = RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
		else {
			vx = -RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
			flagRight = false;
		};
		if (Yplayer >= y)
			vy = RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
		else {
			vy = -RandomFloat(EYEBALL_FLY_SPEED - 0.05, EYEBALL_FLY_SPEED + 0.05);
			flagDown = false;
		}
	}
	else if (decision == 2) {
		if (Xplayer >= x)
			vx = RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03);
		else {
			vx = -RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03);
			flagRight = false;
		};
		if (Yplayer < y)
			vy = RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03);
		else {
			vy = -RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03);
			flagDown = false;
		}
	}
	stopSpeed = RandomFloat(EYEBALL_STOP_SPEED - 0.001, EYEBALL_STOP_SPEED + 0.001);
	flagMove = true;
	ShootPlayer();
}

void CEyeball::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			if (flagRight)
				flagRight = false;
			else flagRight = true;
		};
		if (coEvent->ny != 0)
		{
			vy = -oldVY;
			if (flagDown)
				flagDown = false;
			else flagDown = true;
		};
	}
}


void CEyeball::ShootPlayer()
{
	float dirX, dirY; // direction to the player
	CGameObjectBehaviour::CalcDirectionToPlayer(this, dirX, dirY);

	float Xplayer, Yplayer;
	CGameObjectBehaviour::CalcBoundingBoxCenter(CGame::GetInstance()->GetCurrentPlayer(), Xplayer, Yplayer);

		CBullet_Eyeball* bullet = new CBullet_Eyeball(0, 0, 0, dirX, dirY);
		CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);
}

void CEyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	moveTimer->Update(dt);
	if (flagMove) {
		if (flagRight) {
			vx -= stopSpeed;
			if (vx <= -RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03) *0.9f) {
				vx = 0;
				vy = 0;
				flagMove = false;
			}
		}
		else {
			vx += stopSpeed;
			if (vx >= RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03) * 0.9f) {
				vx = 0;
				vy = 0;
				flagMove = false;
			}
		}
		if (flagDown && flagMove) {
			vy -= stopSpeed;
			if (vy <= -RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03) * 0.9f) {
				vy = 0;
				vx = 0;
				flagMove = false;
			}
		}
		else {
			vy += stopSpeed;
			if (vy >= RandomFloat(EYEBALL_FLY_SPEED - 0.03, EYEBALL_FLY_SPEED + 0.03) * 0.9f) {
				vy = 0;
				vx = 0;
				flagMove = false;
			}
		}
	}
	if (!flagMove) {
		vx = 0;
		vy = 0;
	}
	CEnemy::Update(dt, coObjs);
}

void CEyeball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + EYEBALL_BOUNDBOX_OFFSETX;
	right = left + EYEBALL_BOUNDBOX_WIDTH;
	top = y + EYEBALL_BOUNDBOX_OFFSETY;
	bottom = top + EYEBALL_BOUNDBOX_HEIGHT;
}

void CEyeball::HandleTimerTick(LPTIMER sender)
{
	if (sender == moveTimer)
	{
		approach();
	}
}

