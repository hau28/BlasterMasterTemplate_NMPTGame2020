#include "Bullet_Skull.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"

CBullet_Skull::CBullet_Skull(float x, float y, int sectionId, float dirX, float dirY) : CBullet::CBullet(CLASS_LARGE_GRAY_BULLET, x, y, sectionId, false)
{
	float nx, ny;
	CGameObjectBehaviour::NormalizeVector2(dirX, dirY, nx, ny);

	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);

	float module = sqrt(pow(Xplayer - dirX, 2) + pow(Yplayer - dirY, 2));

	float distanceX = Xplayer - dirX;
	float distanceY = Yplayer - dirY;

	vx = (float)(distanceX / module * 2)/16;
	vy =(float) (distanceY / module * 3)/16;

	explodeTimer = new CTimer(this, TIME_TO_EXPLODE, 1);
	explodeTimer->Start();
}

void CBullet_Skull::UpdateVelocity(DWORD dt)
{
	vy += BULLET_SKULL_GRAVITY;
	vy *= (1 - BULLET_SKULL_FRICTION);

	// Update()
	// vì t lười thêm update nên thôi t spam vô đây tạm nha
	// CuteTN reply: Ok, mốt có lười thì thêm vào UpdateVelocity để nó call 1 lần thôi nha, HandleCollision với HandleOverlap nó được call nhiều lần
	if (!checkObjInCamera(this, SCREEN_EXTEND_OFFSET_DEFAULT))
	{
		CGameObjectBehaviour::RemoveObject(this);
	}

	// CuteTN Note: Tao cũng lười .-.
	explodeTimer->Update(dt);
}

void CBullet_Skull::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
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
			float oldVy = vy;

			CGameObjectBehaviour::BlockObject(dt, coEvent);
			
			// CuteTN Note: bouncing logic
			if (coEvent->ny < 0)
				vy = -oldVy;

			break;
		}
		}
	}
}

void CBullet_Skull::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (overlappedObj == CGame::GetInstance()->GetCurrentPlayer())
		Explode(CLASS_LARGE_EXPLOSION_SIDEVIEW);
}

void CBullet_Skull::HandleTimerTick(CTimer* sender)
{
	Explode(CLASS_LARGE_EXPLOSION_SIDEVIEW);
}

CBullet_Skull::~CBullet_Skull()
{
	CBullet::~CBullet();
	delete explodeTimer;
}


