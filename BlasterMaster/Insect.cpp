#include "Insect.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "Sound.h"


CInsect::CInsect(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = INSECT_HEALTHPOINT;

	SetState(INSECT_STATE_FLY_LEFT);

	vx = 0;
	vy = INSECT_MOVEUP_SPEED_Y;

	timeUpdateVelocite = GetTickCount();
	buzzTimer = new CTimer(this, BUZZ_DURATION);
}

void CInsect::UpdateVelocity(DWORD dt)
{
	if (!flagtouchwall)
	{
		
		if (vy > 0 && GetTickCount() - timeUpdateVelocite > TIME_UPDATE_MOVEDOWN)
		{
			vy = -INSECT_MOVEUP_SPEED_Y;
			timeUpdateVelocite = GetTickCount();
		}
		if (vy < 0 && GetTickCount() - timeUpdateVelocite > TIME_UPDATE_MOVEUP)
		{
			vy = INSECT_MOVEDOWN_SPEED_Y;
			timeUpdateVelocite = GetTickCount();
		}
	}

	if (vy > 0)
		flagDown = true;
	else
		flagDown = false;
	if (!turnRight)
		vx = INSECT_MOVE_SPEED_X;
	else 
		vx = -INSECT_MOVE_SPEED_X;

	/*if (!flagtouchwall ) 
		vy = -INSECT_MOVEUP_SPEED_Y;*/
	UpdateState();
	buzzTimer->Update(dt);

}

void CInsect::UpdateState()
{
	if (vx > 0)
		SetState(INSECT_STATE_FLY_RIGHT);
	else 
		SetState(INSECT_STATE_FLY_LEFT);
}

void CInsect::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
		CGameObjectBehaviour::BlockObject(dt, coEvent);

		if (coEvent->ny > 0)
		{
			flagtouchwall = true;
			vy = INSECT_FALL_SPEED_Y;
			flagDown = true;
		}

		if (coEvent->ny < 0)
		{
			flagtouchwall = false;
			vy = -INSECT_MOVEUP_SPEED_Y;
			flagDown = true;
		}

		if (coEvent->nx < 0)
		{
			turnRight = true;
		}

		if (coEvent->nx > 0)
		{
			turnRight = false;
		}

		UpdateState();
	}
}

void CInsect::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CInsect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + INSECT_BOUNDBOX_OFFSETX;
	right = left + INSECT_BOUNDBOX_WIDTH;
	top = y + INSECT_BOUNDBOX_OFFSETY;
	bottom = top + INSECT_BOUNDBOX_HEIGHT;
}

void CInsect::HandleTimerTick(LPTIMER sender)
{
	if (sender == buzzTimer)
	{
		if (flagDown) {
			Sound::getInstance()->play(BUZZ, false, 1);
		}
	}

}


