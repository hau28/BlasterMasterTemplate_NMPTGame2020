#include "Dome.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "Bullet.h"

CDome::CDome(int classId, int x, int y, int sectionId, int rotation, bool isClockwise, int animsId): CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	this->rotation = rotation;
	this->isClockwise = isClockwise;

	UpdateState();
}

void CDome::UpdateVelocity(DWORD dt)
{
	if (!flagShooting && CheckShootingCondition())
	{
		PrepareShooting();
	}

	if (!flagShooting)
	{
		vx = vy = 0;

		AddStickyForce();
		AddMovingSpeed();
	}
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

			if (flagShooting)
				EndShooting();

			break;
		}
		}
	}

	if (dynamic_cast<LPBULLET>(obj))
	{
		LPBULLET bullet = dynamic_cast<LPBULLET>(obj);

		if (bullet->isFriendly)
		{
			// make explosion effect and destroy this gameobject
			this->TakeDamage(10);
			// remove the bullet from section
			CGameObjectBehaviour::RemoveObject(obj);
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
	
	if(PrepareShootingTimer && flagShooting)
		PrepareShootingTimer->Update(dt);

	UpdateVelocity(dt);

	ResolveInteractions(dt, coObjs);

	HandleRotateOnBlockCorner();

	UpdatePosition(dt);

	UpdateState();

	flashingEffect->Update(dt);
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

	if (flagShooting)
		return;

	bool isConvexCorner = !(flagTouchHorizontalBlock && flagTouchVerticalBlock);

	int moreRotation = (isClockwise ? 1 : -1) * (isConvexCorner ? -1 : 1);
	this->rotation = AddToRotation(this->rotation, moreRotation); 
}

bool CDome::CheckShootingCondition()
{
	LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentPlayer();
	if (!player)
		return false;

	float pcx, pcy; // Player's center position
	CGameObjectBehaviour::CalcBoundingBoxCenter(player, pcx, pcy);
	float dl, dt, dr, db; // Dome's box
	this->GetBoundingBox(dl, dt, dr, db);

	// the actual direction to the player at this frame
	int toPlayerDirX = 0, toPlayerDirY = 0;

	if (dr < pcx)
		toPlayerDirX = 1;
	if (pcx < dl)
		toPlayerDirX = -1;
	if (db < pcy)
		toPlayerDirY = 1;
	if (pcy < dt)
		toPlayerDirY = -1;

	int currentDirX, currentDirY;
	CalcDirectionVector(this->rotation, currentDirX, currentDirY);

	return (currentDirX == toPlayerDirX) && (currentDirY == toPlayerDirY);
}

void CDome::PrepareShooting()
{
	if (flagShooting)
		return;

	if (!PrepareShootingTimer)
	{
		PrepareShootingTimer = new CTimer(this, 500, 1);
	}
	
	PrepareShootingTimer->Start();

	vx = vy = 0;
	flagShooting = true;
}

void CDome::StartShooting()
{
	int shootDirX, shootDirY;
	CalcDirectionVector(this->rotation, shootDirX, shootDirY);

	vx = shootDirX * DOME_SHOOTING_SPEED;
	vy = shootDirY * DOME_SHOOTING_SPEED;
}

void CDome::EndShooting()
{
	flagFirstLand = false;
	flagShooting = false;
	isClockwise = rand() % 2;
	this->rotation = AddToRotation(this->rotation, 2);
}

void CDome::HandleTimerTick(CTimer* sender)
{
	if (sender == PrepareShootingTimer)
	{
		StartShooting();
	}
}

void CDome::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + DOME_BOUNDBOX_OFFSETX;
	right = left + DOME_BOUNDBOX_WIDTH;
	top = y + DOME_BOUNDBOX_OFFSETY;
	bottom = top + DOME_BOUNDBOX_HEIGHT;
}
