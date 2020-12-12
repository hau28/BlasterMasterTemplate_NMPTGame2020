#include "Dome.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

CDome::CDome(int classId, int x, int y, int sectionId, int rotation, bool isClockwise, int animsId): CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	this->rotation = rotation;
	this->isClockwise = isClockwise;

	UpdateState();
}

void CDome::UpdateVelocity(DWORD dt)
{
	vx = vy = 0;
	AddStickyForce();
	AddMovingSpeed();
}

void CDome::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (dynamic_cast<LPTILE_AREA>(obj))
	{
		LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);

		switch (tileArea->classId)
		{
		case CLASS_TILE_BLOCKABLE:
		case CLASS_TILE_PORTAL:
		{
			CGameObjectBehaviour::BlockObject(dt, coEvent);

			if (coEvent->nx != 0)
				flagTouchVerticalBlock = true;

			if (coEvent->ny != 0)
				flagTouchHorizontalBlock = true;

			flagFirstLand = true;

			break;
		}
		}
	}
}

void CDome::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CDome::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	flagTouchHorizontalBlock = false;
	flagTouchVerticalBlock = false;

	UpdateVelocity(dt);

	ResolveInteractions(dt, coObjs);

	HandleRotateOnBlockCorner();

	UpdatePosition(dt);

	UpdateState();
}

void CDome::UpdateState()
{
	int newState;
	switch (rotation)
	{
	case ROTATION_UP:
		newState = flagShooting ? DOME_STATE_SHOOT_UP : DOME_STATE_UP;
		break;

	case ROTATION_RIGHT:
		newState = flagShooting ? DOME_STATE_SHOOT_RIGHT : DOME_STATE_RIGHT;
		break;

	case ROTATION_DOWN:
		newState = flagShooting ? DOME_STATE_SHOOT_DOWN : DOME_STATE_DOWN;
		break;

	case ROTATION_LEFT:
		newState = flagShooting ? DOME_STATE_SHOOT_LEFT : DOME_STATE_LEFT;
		break;

	default:
		return;
	}

	SetState(newState);
}

int CDome::AddToRotation(int rotation, int moreRotate)
{
	int result = ((rotation + moreRotate) % 4 + 4) % 4;
	return result;
}

void CDome::CalcDirectionVector(int rotation, int& x, int& y)
{
	x = y = 0;
	switch (rotation)
	{
	case ROTATION_UP:
		y = -1;
		break;

	case ROTATION_RIGHT:
		x = 1;
		break;

	case ROTATION_DOWN:
		y = 1;
		break;

	case ROTATION_LEFT:
		x = -1;
		break;

	default:
		break;
	}
}

void CDome::AddStickyForce()
{
	int opositeRotation = AddToRotation(this->rotation, 2);
	int stickyDirX, stickyDirY;
	CalcDirectionVector(opositeRotation, stickyDirX, stickyDirY);

	vx += stickyDirX * DOME_STICKY_FORCE;
	vy += stickyDirY * DOME_STICKY_FORCE;
}

void CDome::AddMovingSpeed()
{
	int movingRotation = AddToRotation(this->rotation, isClockwise ? -1 : 1); // turn backward the current rotation direction
	int movingDirX, movingDirY;
	CalcDirectionVector(movingRotation, movingDirX, movingDirY);

	vx += movingDirX * DOME_MOVING_SPEED;
	vy += movingDirY * DOME_MOVING_SPEED;
}

void CDome::HandleRotateOnBlockCorner()
{
	if (flagTouchHorizontalBlock ^ flagTouchVerticalBlock)
		return;

	// CuteTN Note: The dome must have landed on something first before taking this action
	if (!flagFirstLand)
		return;

	bool isConvexCorner = !(flagTouchHorizontalBlock && flagTouchVerticalBlock);

	int moreRotation = (isClockwise ? 1 : -1) * (isConvexCorner ? -1 : 1);
	this->rotation = AddToRotation(this->rotation, moreRotation); // turn backward the current rotation direction
}

void CDome::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + DOME_BOUNDBOX_OFFSETX;
	right = left + DOME_BOUNDBOX_WIDTH;
	top = y + DOME_BOUNDBOX_OFFSETY;
	bottom = top + DOME_BOUNDBOX_HEIGHT;
}
