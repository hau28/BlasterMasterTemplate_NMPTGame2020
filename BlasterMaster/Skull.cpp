#include "Skull.h"
#include "TileArea.h"
#include "Sophia.h"
#include "GameObjectBehaviour.h"
#include "PlayScene.h"
#include "Bullet_Skull.h"


CSkull::CSkull(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{

	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);

	if (Xplayer < x)
	{
		SetState(SKULL_STATE_FLY_LEFT);
		vx = -SKULL_MOVE_SPEED;
	}
	else
	{
		SetState(SKULL_STATE_FLY_RIGHT);
		vx = SKULL_MOVE_SPEED;
	}


	this->isUpdatedWhenOffScreen = true;

	vy = 0;
	healthPoint = SKULL_HEALTHPOINT;
}

void CSkull::UpdateVelocity(DWORD dt)
{
	if (checkObjInCamera(this, SCREEN_EXTEND_OFFSET_DEFAULT))
		flagAppeared = true;

	if (flagAppeared)
	{
		if(!checkObjInCamera(this, SCREEN_EXTEND_OFFSET_DEFAULT))
			CGameObjectBehaviour::RemoveObject(this);
	}

	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);

	if (state != SKULL_STATE_SHOOT_LEFT && state != SKULL_STATE_SHOOT_RIGHT)
	{
		if ((abs(Xplayer - x) < BOX) && !flagshootbullet && Yplayer - y >-20)
		{
			if (vx > 0)
				oldVX = SKULL_MOVE_SPEED;
			else
				oldVX = -SKULL_MOVE_SPEED;

			vx = 0;
			flagshootbullet = true;

			UpdateState();
		}
	}
	
	if ((state == SKULL_STATE_SHOOT_LEFT || state == SKULL_STATE_SHOOT_RIGHT)&& flagshootbullet)
	{
		vy = - SKULL_MOVE_SPEED;
		flagshootbullet = false;

		UpdateState();
	}
}

void CSkull::UpdateState()
{

	if (flagtouchwall|| (vx == 0 && vy == 0))
	{
		if (vx > 0)
			SetState(SKULL_STATE_FLY_LEFT);
		else
			SetState(SKULL_STATE_FLY_RIGHT);

		flagtouchwall = false;
	}

	if (flagshootbullet )
	{
		if (oldVX > 0)
			SetState(SKULL_STATE_SHOOT_LEFT);
		else
			SetState(SKULL_STATE_SHOOT_RIGHT);

		DropBullet();
	}

}

void CSkull::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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

				if (coEvent->ny!= 0 )
				{
					vy = 0;

					if (firstshoot  != 0)
					{
						vx = oldVX;
						firstshoot++;
					}
					else
						firstshoot++;
				
					UpdateState();
				};

				if (coEvent->nx > 0)
				{
					vx = SKULL_MOVE_SPEED;
					flagtouchwall = true;

					UpdateState();
				};

				if (coEvent->nx < 0)
				{
					vx = -SKULL_MOVE_SPEED;
					flagtouchwall = true;

					UpdateState();
				}

				break;
			}
		}
	}
}

void CSkull::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// Chibi cute
}

void CSkull::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + SKULL_BOUNDBOX_OFFSETX;
	right = left + SKULL_BOUNDBOX_WIDTH;
	top = y + SKULL_BOUNDBOX_OFFSETY;
	bottom = top + SKULL_BOUNDBOX_HEIGHT;
}
void CSkull::DropBullet()
{
	float dirX, dirY;
	CGameObjectBehaviour::CalcDirecttionToPlayer(this, dirX, dirY);

	CBullet_Skull* bullet = new CBullet_Skull(0, 0, 0, dirX , dirY );
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);
}


