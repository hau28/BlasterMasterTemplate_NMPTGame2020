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
};

