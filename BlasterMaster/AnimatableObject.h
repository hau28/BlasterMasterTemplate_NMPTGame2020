#pragma once
#include "GameObject.h"
#include "Animations.h"

class CAnimatableObject :
    public CGameObject
{
protected:
	int modifyR = 255;
	int modifyG = 255;
	int modifyB = 255;

public:
	CObjectAnimationHanlders animationHandlers;

	CAnimatableObject() {};
	CAnimatableObject(int classId, int x, int y, int sectionId, int animsId);

	virtual void SetModifyColor(int r, int g, int b);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	/// <summary>
	/// Update the enemy velocity base in ITS BEHAVIOUR
	/// Only update velocity or acceleration
	/// </summary>
	/// <param name="dt"></param>
	virtual void UpdateVelocity(DWORD dt) = 0;

	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj) = 0;
	virtual void CheckOverlaps(vector<LPGAMEOBJECT>* coObjs, vector<LPGAMEOBJECT>& overlappedObjs);
	virtual void HandleOverlaps(vector<LPGAMEOBJECT>* overlappedObjs);

	/// <summary>
	/// set the position to move the enemy a bit so that it won't hit the blockable walls
	/// </summary>
	/// <param name="coObjs"></param>
	virtual void DeoverlapWithBlockableTiles(vector<LPGAMEOBJECT>* coObjs);

	/// <summary>
	/// check and filter out all the collision events that happen in the current frame
	/// </summary>
	/// <param name="dt"></param>
	/// <param name="coObjs"></param>
	virtual void CheckCollision(DWORD dt, vector<LPGAMEOBJECT>* coObjs, vector<LPCOLLISIONEVENT>& coEvents);

	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent) = 0;

	/// <para> CuteTN note:  </para>
	/// this function handle check all the collision with wall object (blockable that was collided horizontally
	/// why? Checking these collision first to hot fix a collision bug just like in the old Mario template
	virtual void HandleCollisionWithWalls(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents);

	/// <para> CuteTN note:  </para>
	/// this function handle check all the collision with blockable tiles
	/// why? This is more priority than any other game objects
	virtual void HandleCollisionWithBlockableTiles(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents);

	/// <summary>
	/// Handle all the collision events
	/// Only update velocity and acceleration
	/// </summary>
	/// <param name="dt"></param>
	/// <param name="coObjs"></param>
	virtual void HandleCollisions(DWORD dt, vector<LPCOLLISIONEVENT>* coEvents);

	/// <summary>
	/// Handle interacting with everything, including collision (check+handle), overlapping (check+handle)...
	/// </summary>
	virtual void ResolveInteractions(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	/// <summary>
	/// Update position base on velocity
	/// </summary>
	/// <param name="dt"></param>
	virtual void UpdatePosition(DWORD dt);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	virtual void Render(float offsetX, float offsetY);

	virtual ~CAnimatableObject();
};

