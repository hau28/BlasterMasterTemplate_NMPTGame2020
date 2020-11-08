#pragma once
#include "GameObject.h"
#include "Animations.h"

class CAnimatableObject :
    public CGameObject
{
private:
	CObjectAnimationHanlders animationHandlers;

public:
	CAnimatableObject() {};
	CAnimatableObject(int classId, int x, int y, int animsId);

	// CuteTN To do: gonna implement this function later baby
	void UpdateAnimation(DWORD dt);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	void Render();

	// void SetAnimationHandlers(CObjectAnimationHanlders animHanlders) { this->animationHanlders = animHanlders; }
};

