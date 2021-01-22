#include "CollisionSolver.h"

void CCollisionSolver::CalcPotentialCollisions(LPGAMEOBJECT thisObject, vector<CGameObject*>* coObjects, vector<LPCOLLISIONEVENT>& coEvents, DWORD dt)
{
	coEvents.clear();

	if (coObjects == nullptr)
		return;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = CCollisionSolver::SweptAABBEx(thisObject, coObjects->at(i), dt);

		if (e->timeEntry > 0 && e->timeEntry <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

LPCOLLISIONEVENT CCollisionSolver::SweptAABBEx(LPGAMEOBJECT thisObject, LPGAMEOBJECT coObject, DWORD dt)
{
	float t, nx, ny;

	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox

	thisObject->GetBoundingBox(ml, mt, mr, mb);
	coObject->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	
	// static object (that object)
	float svx, svy, sdx, sdy;
	coObject->GetSpeed(svx, svy);
	sdx = svx * dt;
	sdy = svy * dt;

	// this object
	float dx, dy, vx, vy;
	thisObject->GetSpeed(vx, vy);
	dx = dt * vx;
	dy = dt * vy;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = dx - sdx;
	float rdy = dy - sdy;

	SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	// CCollisionEvent * e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	CCollisionEvent* e = new CCollisionEvent(thisObject, coObject, t, rdx, rdy, nx, ny);
	return e;
}

#pragma region Collision with sweep AABB
/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void CCollisionSolver::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}
	else
		dx_entry = dx_exit = 0;


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}
	else 
		dy_entry = dy_exit = 0;

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}
#pragma endregion

bool CCollisionSolver::IsOverlapped(float l1, float t1, float r1, float b1, float l2, float t2, float r2, float b2, float& l, float& t, float& r, float& b)
{
	// intersect rectangle
	l = max(l1, l2);
	r = min(r1, r2);
	t = max(t1, t2);
	b = min(b1, b2);

	return (l <= r) && (t <= b);
}


bool CCollisionSolver::IsOverlapped(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2, float& l, float& t, float& r, float& b)
{
	float l1, t1, r1, b1;
	float l2, t2, r2, b2;

	obj1->GetBoundingBox(l1, t1, r1, b1);
	obj2->GetBoundingBox(l2, t2, r2, b2);

	return IsOverlapped(l1, t1, r1, b1, l2, t2, r2, b2, l, t, r, b);
}

bool CCollisionSolver::IsTouchingSophia (LPGAMEOBJECT obj1, LPGAMEOBJECT obj2)
{
	float l1, t1, r1, b1;
	float l2, t2, r2, b2;

	obj1->GetBoundingBox(l1, t1, r1, b1);
	obj2->GetBoundingBox(l2, t2, r2, b2);
	if ((b1 < b2-2)||(t2+2<t1)) return false;
	return
		l1 <= l2 &&
		r1 >= r2;
}

bool CCollisionSolver::IsOverlapped(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2)
{
	float l, r, t, b;
	return IsOverlapped(obj1, obj2, l, t, r, b);
}


void CCollisionSolver::DeOverlap(LPGAMEOBJECT movableObj, LPGAMEOBJECT staticObj, float& dx, float& dy)
{
	const float eps = 1e-9;
	dx = dy = 0;

	float l, t, r, b;

	float l1, t1, r1, b1;
	float l2, t2, r2, b2;

	movableObj->GetBoundingBox(l1, t1, r1, b1);
	staticObj->GetBoundingBox(l2, t2, r2, b2);

	if (IsOverlapped(movableObj, staticObj))
	{
		// now try moving in 4 directions
		float dl = ceil(r1 - l2);
		float dr = ceil(r2 - l1);
		float dt = ceil(b1 - t2);
		float db = ceil(b2 - t1);
		float dmin = min(min(dl, dr), min(dt, db));

		if (abs(dmin - dl) < eps)
			dx = -dl;
		else if (abs(dmin - dr) < eps)
			dx = dr;
		else if (abs(dmin - dt) < eps)
			dy = -dt;
		else if (abs(dmin - db) < eps)
			dy = db;
	}
}
