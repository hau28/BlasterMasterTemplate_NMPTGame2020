#include "Mine.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"
#include "Sophia.h"
#include "JasonSideview.h"
#include "Bullet_Mine.h"
#include "Utils.h"
#include "Sound.h"



CMine::CMine(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = MINE_HEALTHPOINT;

	SetState(MINE_STATE_LIVE);
	vy = MINE_GRAVITY;
}

void CMine::ShootBullets()
{
	int numberOfBullet = rand() % MINE_MAX_NUMBER_OF_BULLETS;

	// CuteTN Note: this is a dirty way to make bigger numbers have more chance to appear
	numberOfBullet = max(numberOfBullet, rand() % MINE_MAX_NUMBER_OF_BULLETS);

	for (int i = 0; i < numberOfBullet; i++)
	{
		float x_factor = RandomFloat(-MINE_BULLET_RANGE_X, MINE_BULLET_RANGE_X);
		float y_factor = RandomFloat(-MINE_BULLET_RANGE_Y, 0);

		CBullet_Mine* bullet = new CBullet_Mine(0, 0, 0, x_factor, y_factor);
		CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);
	}
}

void CMine::UpdateVelocity(DWORD dt)
{
	// Thy cute
	vy = MINE_GRAVITY;
	if (animationHandlers[state]->currentFrameIndex == 1)
		flagSound = true;
	if (animationHandlers[state]->currentFrameIndex == 0 && flagSound) {
		Sound::getInstance()->play(MINE, false, 1);
		flagSound = false;
	}
}

void CMine::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
				break;
			}
		}
	}
}

void CMine::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (overlappedObj == CGame::GetInstance()->GetCurrentPlayer())
	{
		Explode();
	}
}

void CMine::Explode()
{
	
	Sound::getInstance()->play(ENEMY_DIE, false, 1);
	CEnemy::Explode();
	ShootBullets();
}

void CMine::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + MINE_BOUNDBOX_OFFSETX;
	right = left + MINE_BOUNDBOX_WIDTH;
	top = y + MINE_BOUNDBOX_OFFSETY;
	bottom = top + MINE_BOUNDBOX_HEIGHT;
}




