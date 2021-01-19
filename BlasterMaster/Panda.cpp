#include "Panda.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  
#include "Sound.h"


CPanda::CPanda(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = PANDA_HEALTHPOINT;
	Sound::getInstance()->stop(PANDA_MOVING);
	SetState(PANDA_STATE_WALK_RIGHT);
	vyMax = PANDA_MAX_FALL_SPEED;
	stepTimer = new CTimer(this, STEP_DURATION);
};

void CPanda::HandleTimerTick(LPTIMER sender) {
	if (sender == stepTimer)
	{
		if (!flagOnAir) {
			Sound::getInstance()->play(PANDA_MOVING, false, 1);
		}
	}
}

void CPanda::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + PANDA_BOUNDBOX_OFFSETX;
	right = left + PANDA_BOUNDBOX_WIDTH;
	top = y + PANDA_BOUNDBOX_OFFSETY;
	bottom = top + PANDA_BOUNDBOX_HEIGHT;
}

void CPanda::checkChangePositionPlayer()
{
	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);
	if (abs(Xplayer - flagPlayerX) <= 2)
	{
		flagPlayerX = Xplayer;
		return;
	}
	flagPlayerX = Xplayer;
	isGoingToPlayer = true;
}

void CPanda::UpdateVelocity(DWORD dt)
{
	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);
	
	if (!flagOnAir && vy >= 0)
	{
		if (isGoingToPlayer)
		{
			if (Xplayer > x)
				vx = PANDA_MOVE_SPEED;
			else
			if (Xplayer < x)
				vx = -PANDA_MOVE_SPEED;
		}
		else
		{
			if (abs(destinationX - x) <= 1)
			{
				isGoingToPlayer = true;
			}
			else
				if (destinationX > x)
					vx = PANDA_MOVE_SPEED;
				else
				if (destinationX < x)
					vx = -PANDA_MOVE_SPEED;
		}
	}

	if (abs(vy - 0.04) <= 0.0001)
		if (Xplayer > x)
			vx = PANDA_MOVE_SPEED;
		else
			if (Xplayer < x)
				vx = -PANDA_MOVE_SPEED;
	
	vy += PANDA_GRAVITY;
	vy = min(vy, PANDA_MAX_FALL_SPEED);	
}

void CPanda::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		CGameObjectBehaviour::BlockObject(dt, coEvent);

		if (coEvent->ny < 0)
		{
			flagOnAir = false;
			if (flagTouchWall)
			{
				Sound::getInstance()->play(PANDA_JUMP_SOUND, false, 1);
				vy = -PANDA_JUMP;
			}
		}

		if (coEvent->nx != 0)
		{
			flagTouchWall = true;

			if (!isGoingToPlayer)
				isGoingToPlayer = true;
		}
	}
}

void CPanda::checkDeoverlapPlayer()
{
	if (flagOnAir)
		return;
	float boundPlayerLeft, boundPlayerTop, boundPlayerRight, boundPlayerBottom;
	float boundPandaLeft, boundPandaTop, boundPandaRight, boundPandaBottom;
	CGame::GetInstance()->GetCurrentPlayer()->GetBoundingBox(boundPlayerLeft, boundPlayerTop, boundPlayerRight, boundPlayerBottom);
	this->GetBoundingBox(boundPandaLeft,boundPandaTop,boundPandaRight,boundPandaBottom);

	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);

	float centerPlayer = (boundPlayerLeft + boundPlayerRight) / 2;

	if ((boundPandaLeft < centerPlayer && centerPlayer < boundPandaRight))
	{
		if (isGoingToPlayer)
		{
			isGoingToPlayer = false;
			float dau = rand() % 2;
			float distance = rand() % 20 + 10;

			if (dau == 1)
				destinationX = x + distance;
			else
				destinationX = x - distance;
		}
	}
}

void CPanda::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	stepTimer->Update(dt);
	UpdateVelocity(dt);
	flagOnAir = true;
	flagTouchWall = false;
	ResolveInteractions(dt, coObjs);
	UpdatePosition(dt);
	checkChangePositionPlayer();
	checkDeoverlapPlayer();
	if (vx > 0)
		SetState(PANDA_STATE_WALK_RIGHT);
	if (vx < 0)
		SetState(PANDA_STATE_WALK_LEFT);
	// CuteTN
	flashingEffect->Update(dt);
}	
