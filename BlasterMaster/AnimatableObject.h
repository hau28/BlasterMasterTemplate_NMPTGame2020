#pragma once
#include "GameObject.h"
#include "Animations.h"

class CAnimatableObject :
    public CGameObject
{
public:
	CObjectAnimationHanlders animationHandlers;

	CAnimatableObject() {};
	CAnimatableObject(int classId, int x, int y, int animsId);

	// CuteTN To do: gonna implement this function later baby
	void UpdateAnimation(DWORD dt) {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs) {};
	virtual void Render();

	// void SetAnimationHandlers(CObjectAnimationHanlders animHanlders) { this->animationHanlders = animHanlders; }
};

