#include "Ship.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Bullet_Ship.h"

void CShip::UpdateState()
{
	if (vx < 0)
		SetState(SHIP_STATE_FLY_LEFT);
	else
		SetState(SHIP_STATE_FLY_RIGHT);
}

CShip::CShip(int classId, int x, int y, int sectionId, int initLeft, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	vx = SHIP_MOVE_SPEED;
	vy = 0;

	if (initLeft)
		vx = -vx;

	UpdateState();
	
	singleShotTimer = new CTimer(this, DELAY_BETWEEN_SHOTS, SHOT_PER_SHOOTING_PHASE);
	shootPhaseTimer = new CTimer(this, DELAY_BETWEEN_SHOOTING_PHASES);
}

void CShip::UpdateVelocity(DWORD dt)
{
	// Thy cute
}

void CShip::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			{
				vx = coEvent->nx * SHIP_MOVE_SPEED;
				UpdateState();
			}
				
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

void CShip::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// Chibi cute
}

void CShip::ShootPlayer()
{
	float dirX, dirY; // direction to the player
	CGameObjectBehaviour::CalcDirecttionToPlayer(this, dirX, dirY);

	CBullet_Ship* bullet = new CBullet_Ship(0, 0, 0, dirX, dirY);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);
}

void CShip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	singleShotTimer->Update(dt);
	shootPhaseTimer->Update(dt);

	CEnemy::Update(dt, coObjs);
}

void CShip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + SHIP_BOUNDBOX_OFFSETX;
	right = left + SHIP_BOUNDBOX_WIDTH;
	top = y + SHIP_BOUNDBOX_OFFSETY;
	bottom = top + SHIP_BOUNDBOX_HEIGHT;
}

void CShip::HandleTimerTick(LPTIMER sender)
{
	if (sender == singleShotTimer)
	{
		ShootPlayer();
	}

	if (sender == shootPhaseTimer)
	{
		singleShotTimer->Start();
	}
}
