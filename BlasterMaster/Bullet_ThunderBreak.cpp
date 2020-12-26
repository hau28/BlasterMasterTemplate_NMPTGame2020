#include "Bullet_ThunderBreak.h"
#include "Enemy.h"
#include "GameObjectBehaviour.h"

CBullet_ThunderBreak::CBullet_ThunderBreak(float x, float y, int section, int indexInChain) : CBullet(CLASS_THUNDERBREAK, x, y, section, true)
{
	allowOverlapWithBlocks = true;
	isUpdatedWhenOffScreen = true;
	isHiddenByForeground = false;

	SetRandomState();
}

CBullet_ThunderBreak::CBullet_ThunderBreak(float x, float y, int section) : CBullet_ThunderBreak(x, y, section, 0)
{
}

void CBullet_ThunderBreak::SetRandomState()
{
	int x = rand() % 4;

	switch (x)
	{
	case 1: SetState(THUNDERBREAK_STATE_1); return;
	case 2: SetState(THUNDERBREAK_STATE_2); return;
	case 3: SetState(THUNDERBREAK_STATE_3); return;
	case 4: SetState(THUNDERBREAK_STATE_4); return;
	}
}

void CBullet_ThunderBreak::UpdateVelocity(DWORD dt)
{
}

void CBullet_ThunderBreak::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CBullet_ThunderBreak::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (dynamic_cast<CEnemy*>(overlappedObj))
	{
		CEnemy* enemy = dynamic_cast<CEnemy*>(overlappedObj);
		CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(this, enemy, true);
	}
}

void CBullet_ThunderBreak::CreateNextUnit()
{
}

void CBullet_ThunderBreak::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}
