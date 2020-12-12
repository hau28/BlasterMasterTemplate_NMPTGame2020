#pragma once
#include "GameObject.h"

class CCollisionSolver
{
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	/// <summary>
	/// <para> Extension of original SweptAABB to deal with two moving objects </para>
	/// </summary>
	/// <param name="coObject"></param>
	/// <returns></returns>
	static LPCOLLISIONEVENT CCollisionSolver::SweptAABBEx(LPGAMEOBJECT thisObject, LPGAMEOBJECT coObject, DWORD dt);

public:
	/// <summary>
	/// <para> Calculate potential collisions with the list of colliable objects </para>
	/// </summary>
	/// <param name="coObjects:"> The list of colliable objects </param>
	/// <param name="coEvents:"> List of potential collisions </param>
	static void CalcPotentialCollisions(LPGAMEOBJECT thisObject, vector<CGameObject*>* coObjects, vector<LPCOLLISIONEVENT>& coEvents, DWORD dt);

	static bool IsOverlapped(float l1, float t1, float r1, float b1, float l2, float t2, float r2, float b2, float& l, float& t, float& r, float& b);

	/// <summary>
	/// check if 2 game objects' bounding box is overlapped or not
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	/// <returns></returns>
	static bool IsOverlapped(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2);

	/// <summary>
	/// check if 2 game objects' bounding box is overlapped or not, also return the intersect rectangle
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	/// <returns></returns>
	static bool IsOverlapped(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2, float& l, float& t, float& r, float& b);


	/// <summary>
	/// slightly move a non-static object so that 2 objects are no longer overlapped
	/// </summary>
	/// <param name="movableObj"></param>
	/// <param name="staticObj"></param>
	/// <param name="dx, dy">direction to move to deoverlap</param>
	/// <returns></returns>
	static void DeOverlap(LPGAMEOBJECT movableObj, LPGAMEOBJECT staticObj, float& dx, float& dy);

	static bool IsTouchingSophia(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2);
};

