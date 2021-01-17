#include "Enemy.h"
#include "GameObjectBehaviour.h"
#include "Item.h"
#include "Sound.h"

CEnemy::CEnemy(int classId, int x, int y, int sectionId, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId, animsId)
{
	flashingEffect = new CObjectFlashingEffectPlayer(this, &flashingColors, ENEMY_FLASHING_FRAME_DURATION);
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CAnimatableObject::Update(dt, coObjs);
	flashingEffect->Update(dt);
}

bool CEnemy::IsBlockableObject(LPGAMEOBJECT obj)
{
	if (obj->classId == CLASS_TILE_PORTAL || obj->classId == CLASS_TILE_PORTAL_O)
		return true;

	return CAnimatableObject::IsBlockableObject(obj);
}

int CEnemy::GetHealthPoint()
{
	return healthPoint;
}

void CEnemy::TakeDamage(int damage)
{
	// if the bullet have no damage left
	if (damage == 0)
		return;

	if (damage < healthPoint)
	{
		Sound::getInstance()->play(ENEMY_GOT_HIT, false, 1);
		healthPoint -= damage;
		PlayFlashingEffect();
	}
	else
	{
		Sound::getInstance()->play(ENEMY_DIE, false, 1);
		healthPoint = 0;
		Explode();
		DropItem();
	}
}

void CEnemy::DropItem()
{
	// CuteTN Confess: dirty function to randomly generate item
	const int EXPECTED_NONE = 30;
	const int EXPECTED_POWER = 10;
	const int EXPECTED_HOVER = 1;

	int sum = EXPECTED_NONE + EXPECTED_POWER + EXPECTED_HOVER;
	int startRangeNone = 0;
	int startRangePower = startRangeNone + EXPECTED_NONE;
	int startRangeHover = startRangePower + EXPECTED_POWER;
	
	int x = rand() % sum;
	LPITEM item = nullptr;

	if (x >= startRangeHover)
		item = new CItem(CLASS_ITEM_HOVER, 0, 0, 0, false);
	else if (x >= startRangePower)
		item = new CItem(CLASS_ITEM_POWER, 0, 0, 0, false);

	if (item)
	{
		CGameObjectBehaviour::CreateObjectAtCenterOfAnother(item, this);
	}
}

void CEnemy::Explode()
{
	CGameObjectBehaviour::ExplodeAtCenter(this, CLASS_LARGE_EXPLOSION_SIDEVIEW);
}

void CEnemy::PlayFlashingEffect()
{
	if(flashingEffect)
		flashingEffect->Play();
}
