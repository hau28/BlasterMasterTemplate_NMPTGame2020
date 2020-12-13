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

void CEnemy::TakeDamage(int damage)
{
	if (damage < healthPoint)
	{
		healthPoint -= damage;
		PlayFlashingEffect();
	}
	else
	{
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
