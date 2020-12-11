#include "Floater.h"
#include "GameObjectBehaviour.h"

void CFloater::UpdateState()
{
	if (vx < 0)
		SetState(FLOATER_STATE_FLY_RIGHT);
	else
		SetState(FLOATER_STATE_FLY_RIGHT);
}

CFloater::CFloater(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId) 
{
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
};

void CFloater::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;


		switch (obj->classId)
		{
		case CLASS_TILE_BLOCKABLE:
		case CLASS_TILE_PORTAL:
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

			break;
		}

	}
}

void CFloater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + FLOATER_BOUNDBOX_OFFSETX;
	right = left + FLOATER_BOUNDBOX_WIDTH;
	top = y + FLOATER_BOUNDBOX_OFFSETY;
	bottom = top + FLOATER_BOUNDBOX_HEIGHT;
}
