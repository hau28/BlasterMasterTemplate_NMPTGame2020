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

	timestartDrop = GetTickCount();


}

void CBullet_Skull::UpdateVelocity(DWORD dt)
{

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
			CGameObjectBehaviour::BlockObject(dt, coEvent);
			
			if (coEvent->ny != 0)
				vy = BULLET_SKULL_SPEED;
			break;
		}
		}
	}

	// vì t lười thêm update nên thôi t spam vô đây tạm nha
	if (!checkObjInCamera(this, SCREEN_EXTEND_OFFSET_DEFAULT) || GetTickCount() - timestartDrop > TIMETOEXPLODE || CCollisionSolver::IsOverlapped(this, CGame::GetInstance()->GetCurrentPlayer()))
	{
		CGameObjectBehaviour::RemoveObject(this);
		Explode(CLASS_LARGE_EXPLOSION_SIDEVIEW);
	}
}

void CBullet_Skull::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}


