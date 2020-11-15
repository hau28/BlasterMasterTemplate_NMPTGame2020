#include "GameObjectBehaviour.h"

void CGameObjectBehaviour::GetBlocked(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	LPGAMEOBJECT movableObj = coEvent->sourceObject;
	LPGAMEOBJECT blockableObj = coEvent->otherObject;

	float x, y, vx, vy;
	movableObj->GetPosition(x, y);
	movableObj->GetSpeed(vx, vy);
	
	// move the object to the collision position, then set the velocity to 0
	if (coEvent->nx != 0)
	{
		float dx = coEvent->rdx * coEvent->timeEntry / dt;
		x += dx;
		vx = 0;
	}

	if (coEvent->ny != 0)
	{
		float dy = coEvent->rdy * coEvent->timeEntry / dt;
		y += dy;
		vy = 0;
	}

	movableObj->SetPosition(x, y);
	movableObj->SetSpeed(vx, vy);
}
