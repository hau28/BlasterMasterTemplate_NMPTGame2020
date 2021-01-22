#include "GameObject.h"
#include "CollisionSolver.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
}


void CGameObject::RenderBoundingBox(float offsetX, float offsetY)
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

	// CuteTN to do: work on this function later... or maybe never :)
	CGame::GetInstance()->Draw(l + offsetX, t + offsetY, bbox, rect.left, rect.top, rect.right, rect.bottom, 200);
}

CGameObject::~CGameObject()
{

}