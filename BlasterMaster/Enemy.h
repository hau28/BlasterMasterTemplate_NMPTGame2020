#pragma once
#include "AnimatableObject.h"
#include "ObjectFlashingEffectPlayer.h"

class CEnemy : public CAnimatableObject
{
protected:
	const int ENEMY_FLASHING_FRAME_DURATION = 20;
	static vector<Color> flashingColors;
	CObjectFlashingEffectPlayer* flashingEffect = nullptr;

	int healthPoint = 100;
	void Explode();
	void PlayFlashingEffect();

public:
	CEnemy() {};
	CEnemy(int classId, int x, int y, int sectionId, int animsId);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	void TakeDamage(int damage);
};

