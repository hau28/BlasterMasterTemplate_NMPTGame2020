#include "GameObject.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
}

LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coObject)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coObject->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coObject->GetSpeed(svx, svy);

	float sdx = svx*dt;
	float sdy = svy*dt;

	float dx, dy;
	this->GetPositionDifference(dx, dy);

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = dx - sdx;
	float rdy = dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	// CCollisionEvent * e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	CCollisionEvent* e = new CCollisionEvent(this, coObject, t, rdx, rdy, nx, ny);
	return e;
}

void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	if (coObjects == nullptr)
		return;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->timeEntry > 0 && e->timeEntry <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->timeEntry < min_tx && c->nx != 0) {
			min_tx = c->timeEntry; nx = c->nx; min_ix = i; rdx = c->rdx;
		}

		if (c->timeEntry < min_ty  && c->ny != 0) {
			min_ty = c->timeEntry; ny = c->ny; min_iy = i; rdy = c->rdy;
		}
	}

	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
}


void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}

void CGameObject::SolveClassicalMechanics()
{
	vx += ax * dt;
	vy += ay * dt;

	if (vx > 0)
		vx = min(vx, vxMax);
	else
		vx = max(vx, -vxMax);

	if (vy > 0)
		vy = min(vy, vyMax);
	else
		vy = max(vy, -vyMax);

	x += vx * dt;
	y += vy * dt;
}

CGameObject::~CGameObject()
{

}