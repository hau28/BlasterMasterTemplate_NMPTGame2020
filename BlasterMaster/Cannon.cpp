#include "Cannon.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"
#include "Item.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Bullet_Cannon.h"
#include "Sound.h"

void CCannon::UpdateState()
{

}

CCannon::CCannon(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = CANNON_HEALTHPOINT;

	UpdateState();
	flagVertical = true;
	flagHorizontal = true;
};

void CCannon::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
	}
}


void CCannon::ShootHorizontal()
{
	Sound::getInstance()->play(ENEMY_OVERHEAD_SHOOT, false, 1);
	CBullet_Cannon* bullet1 = new CBullet_Cannon(0, 0, 0, 1, 0);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet1, this);

	CBullet_Cannon* bullet2 = new CBullet_Cannon(0, 0, 0, -1, 0);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet2, this);
}

void CCannon::DropItem()
{
	const int EXPECTED_NONE = 40;
	const int EXPECTED_GUN = 60;

	int x = rand() % (EXPECTED_NONE + EXPECTED_GUN);
	if (x >= EXPECTED_NONE)
	{
		LPITEM gunItem = new CItem(CLASS_ITEM_GUN, 0, 0, 0, false);
		CGameObjectBehaviour::CreateObjectAtCenterOfAnother(gunItem, this);
	}
}

void CCannon::ShootVertical()
{
	Sound::getInstance()->play(ENEMY_OVERHEAD_SHOOT, false, 1);
	CBullet_Cannon* bullet1 = new CBullet_Cannon(0, 0, 0, 0, 1);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet1, this);
	CBullet_Cannon* bullet2 = new CBullet_Cannon(0, 0, 0, 0, -1);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet2, this);
}

void CCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (animationHandlers[state]->currentFrameIndex == 2) {
		flagVertical = true;
		if (flagHorizontal) {
			ShootHorizontal();
			flagHorizontal = false;
		}
	}
	if (animationHandlers[state]->currentFrameIndex == 0) {
		flagHorizontal = true;
		if (flagVertical) {
			ShootVertical();
			flagVertical = false;
		}
	}
	CEnemy::Update(dt, coObjs);
}

void CCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + CANNON_BOUNDBOX_OFFSETX;
	right = left + CANNON_BOUNDBOX_WIDTH;
	top = y + CANNON_BOUNDBOX_OFFSETY;
	bottom = top + CANNON_BOUNDBOX_HEIGHT;
}

void CCannon::HandleTimerTick(LPTIMER sender)
{

}
