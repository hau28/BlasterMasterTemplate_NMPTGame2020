#pragma once

#include <Windows.h>
#include <vector>
#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "Textures.h"
#include "Game.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CGameObject
class CGameObject
{
protected:
	/// CuteTN Note: using both dx and vx could be confusing... So I removed it :^)

	/// <summary>
	/// <para> position of GameObject relative to game world (NOT the camera) </para> 
	/// </summary>
	float x, y;

	/// <summary>
	/// velocity of this gameObject
	/// </summary>
	float vx, vy;
	float vxMax, vyMax;

	/// <summary>
	/// acceleration of this gameObject
	/// </summary>
	float ax, ay;

	/// <summary>
	/// <para> don't even understand this properties :) </para>
	/// </summary>
	int nx;	 

	int state = 0;

	/// <summary>
	/// <para> Time elapsed in 1 frame </para>
	/// <para> Dont really know why we need it but OK :) </para>
	/// </summary>
	DWORD dt; 

	int classId;

public: 
	CGameObject();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }

	/// <summary>
	/// <para> (dx,dy) is the distance this gameObject has moved in the current frame. </para>
	/// <para> dx = vx * dt; dy = vy *dt </para>
	/// </summary>
	/// <param name="dx"></param>
	/// <param name="dy"></param>
	void GetPositionDifference(float &dx, float &dy) { dx = vx*dt; dy = vy*dt; }

	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	
	void SetAcceleration(float ax, float ay) { this->ax = ax, this->ay = ay; }
	void GetAcceleration(float &ax, float &ay) { ax = this->ax; ay = this->ay; }
	
	virtual void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	/// <summary>
	/// <para> Evaluate velocity from acceleration, then position by velocity. </para>
	/// <para> with a little knowledge of calculus and classical mechanics, you'd know that: </para>
	/// <para> dx / dt = v </para>
	/// <para> dv / dt = a </para>
	/// <para> WARNING: Base method doesn't calculate gravity. </para>
	/// </summary>
	virtual void SolveClassicalMechanics();

	void RenderBoundingBox();

	/// <summary>
	/// <para> Extension of original SweptAABB to deal with two moving objects </para>
	/// </summary>
	/// <param name="coObject"></param>
	/// <returns></returns>
	LPCOLLISIONEVENT SweptAABBEx(CGameObject* coObject);

	/// <summary>
	/// <para> Calculate potential collisions with the list of colliable objects </para>
	/// </summary>
	/// <param name="coObjects:"> The list of colliable objects </param>
	/// <param name="coEvents:"> List of potential collisions </param>
	void CalcPotentialCollisions(vector<CGameObject*> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	/// <summary>
	/// <para> Getting the actual hitbox (collision box) of a game object. Relative to gameworld, not to the sprite itself. </para>
	/// <para> Should be implemented with O(1) complexity so that we can call it everytime needed without worrying about the performance (otherwise consider using some properties as cache instead?) </para>
	/// <para> WARNING 1: be aware of game object state and direction </para>
	/// <para> WARNING 2: please define a constant whenever you need it, don't spam cancerous numbers into this function! </para>
	/// </summary>
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;

	/// <summary>
	/// <para> Update function is called at most once per frame (conventionally) </para>
	/// <para> Updates this object internal state based on its model (path, physics), interaction with other objects and user input </para>
	/// <para> </para>
	/// </summary>
	/// <param name="dt:">Time elapsed from last frame. be acknowledged that this is NOT A CONSTANT, it's based on real-time</param>
	/// <param name="coObjects:">Collision objects. I've not yet known how can this parameter help, but let us keep it that way...</param>
	virtual void Update(DWORD dt, vector<CGameObject*> *coObjects = nullptr);

	/// <summary>
	/// <para> Render function is called at most once per frame (conventionally) </para>
	/// <para> Render this game object to user screen </para>
	/// <para> WARNING 1: this could cover other game objects and causes visual bugs. The order of rendering is important, too </para>
	/// </summary>
	virtual void Render() = 0;

    int GetClass() { return classId; };

	~CGameObject();
};

typedef CGameObject * LPGAMEOBJECT;
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CCollisionEvent
/// <summary>
/// <para> Stores infomation of a collision event from ONE game object to ANOTHER </para>
/// <para> Based on Swept AABB algorithm </para>
/// </summary>
struct CCollisionEvent
{
	LPGAMEOBJECT sourceObject;
	LPGAMEOBJECT otherObject;

	/// <summary>
	/// <para> Predicted time until colision based on swept AABB Algoithm </para>
	/// <para> Collision on next frame only when: 0&lt;timeEntry&lt;1 </para>
	/// </summary>
	float timeEntry;

	/// <summary>
	/// <para> A small distance to move in order not to collide to otherObject </para> 
	/// <para> Backward of (dx,dy) </para>
	/// <para> Value can only be either -1, 0 or 1 </para>
	/// </summary>
	float nx, ny;

	/// <summary>
	/// <para> Relative movement from sourceObject to otherObject perspective (i.e otherObject is static) </para>
	/// </summary>
	float rdx, rdy;

	CCollisionEvent(LPGAMEOBJECT sourceObject, LPGAMEOBJECT otherObject, float timeEntry, float rdx, float rdy, float nx, float ny)
	{
		this->sourceObject = sourceObject;
		this->otherObject = otherObject;
		this->timeEntry = timeEntry;
		this->rdx = rdx;
		this->rdy = rdy;
		this->nx = nx;
		this->ny = ny;
	}

	/// <summary>
	/// <para> One doesn't have to go to this function definition just to know it compares 2 entryTime, do they? </para>
	/// </summary>
	/// <returns>returns TRUE iff a.timeEntry&lt;b.timeEntry</returns>
	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->timeEntry < b->timeEntry;
	}
};
#pragma endregion
