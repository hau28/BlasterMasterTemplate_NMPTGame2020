#include "Enemy.h"
#include "GameObjectBehaviour.h"

vector<Color> CEnemy::flashingColors = 
{ 
	{255,0,100}, 
	{10,255,10}, 
	{255, 255, 255}, 
	{255,0,100}, 
	{10,255,10}, 
	{255, 255, 255} 
};

CEnemy::CEnemy(int classId, int x, int y, int sectionId, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId, animsId)
{
	flashingEffect = new CObjectFlashingEffectPlayer(this, &flashingColors, ENEMY_FLASHING_FRAME_DURATION);
}

void CEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CAnimatableObject::Update(dt, coObjs);
	flashingEffect->Update(dt);
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
		healthPoint -= damage;
		PlayFlashingEffect();
	}
	else
	{
		healthPoint = 0;
		Explode();
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
