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

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	/// <summary>
	/// Update the enemy velocity base in ITS BEHAVIOUR
	/// Only update velocity or acceleration
	/// </summary>
	/// <param name="dt"></param>
	virtual void UpdateVelocity(DWORD dt) = 0;

	/// <summary>
	/// set the position to move the enemy a bit so that it won't hit the blockable walls
	/// </summary>
	/// <param name="coObjs"></param>
	virtual void Deoverlap(vector<LPGAMEOBJECT>* coObjs);

	/// <summary>
	/// check and filter out all the collision events that happen in the current frame
	/// </summary>
	/// <param name="dt"></param>
	/// <param name="coObjs"></param>
	virtual void CheckCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjs, vector<LPCOLLISIONEVENT>& coEvents);

	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) = 0;

	/// <summary>
	/// Handle all the collision events
	/// Only update velocity and acceleration
	/// </summary>
	/// <param name="dt"></param>
	/// <param name="coObjs"></param>
	virtual void HandleCollisions(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents);

	/// <summary>
	/// Update position base on velocity
	/// </summary>
	/// <param name="dt"></param>
	virtual void UpdatePosition(DWORD dt);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	virtual void Render();
};

