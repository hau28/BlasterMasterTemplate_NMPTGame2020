#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"
#include "Explosion.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

void CGameObjectBehaviour::BlockObject(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	LPGAMEOBJECT movableObj = coEvent->sourceObject;
	LPGAMEOBJECT blockableObj = coEvent->otherObject;

	float x, y, vx, vy;
	movableObj->GetPosition(x, y);
	movableObj->GetSpeed(vx, vy);
	
	// move the object to the collision position, then set the velocity to 0
	if (coEvent->nx != 0)
	{
		float dx = coEvent->rdx * coEvent->timeEntry / dt;
		x += dx;
		vx = 0;
	}

	if (coEvent->ny != 0)
	{
		float dy = coEvent->rdy * coEvent->timeEntry / dt;
		y += dy;
		vy = 0;
	}

	movableObj->SetPosition(x, y);
	movableObj->SetSpeed(vx, vy);

	// direction to deoverlap
	float doX, doY;
	CCollisionSolver::DeOverlap(movableObj, blockableObj, doX, doY);

	movableObj->SetPosition(x + doX, y + doY);
}

void CGameObjectBehaviour::CalcBoundingBoxCenter(LPGAMEOBJECT obj, float& x, float& y)
{
	if (obj == nullptr)
	{
		x = y = 0;
		return;
	}

	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);
	x = (l + r) / 2;
	y = (t + b) / 2;
}

void CGameObjectBehaviour::SetBoundingBoxPos(LPGAMEOBJECT obj, float x, float y)
{
	float l, t, r, b, oX, oY;
	obj->GetBoundingBox(l, t, r, b);
	obj->GetPosition(oX, oY);

	float offsetX = l - oX;
	float offsetY = t - oY;

	obj->SetPosition(x - offsetX, y - offsetY);
}

void CGameObjectBehaviour::SetBoundingBoxCenter(LPGAMEOBJECT obj, float x, float y)
{
	float oX, oY, cX, cY;
	obj->GetPosition(oX, oY);
	CalcBoundingBoxCenter(obj, cX, cY);

	float offsetX = cX - oX;
	float offsetY = cY - oY;

	obj->SetPosition(x - offsetX, y - offsetY);
}

void CGameObjectBehaviour::CalcDirecttionToPlayer(LPGAMEOBJECT obj, float& x, float& y)
{
	if (obj == nullptr)
		return;
	float xO, yO;
	CalcBoundingBoxCenter(obj, xO, yO);

	LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentPlayer();
	
	if (player == nullptr)
		return;
	float xP, yP;
	CalcBoundingBoxCenter(player, xP, yP);

	x = (xP - xO);
	y = (yP - yO);
}

float CGameObjectBehaviour::CalcMagnitudeVector2(float x, float y)
{
	return sqrt(x * x + y * y);
}

void CGameObjectBehaviour::NormalizeVector2(float x, float y, float& nx, float& ny)
{
	float d = CalcMagnitudeVector2(x, y);
	nx = x / d;
	ny = y / d;
}

void CGameObjectBehaviour::TransformBoundBox(float offsetX, float offsetY, float width, float height, float spriteWidth, float spriteHeight, float& newOffsetX, float& newOffsetY, float& newWidth, float& newHeight, bool flipX, bool flipY)
{
	newOffsetX = offsetX;
	newOffsetY = offsetY;
	newWidth = width;
	newHeight = height;

	if (flipX)
	{
		newOffsetX = spriteWidth - offsetX - width;
	}

	if (flipY)
	{
		newOffsetY = spriteHeight - offsetY - height;
	}
}

void CGameObjectBehaviour::CreateExplosion(int explosionClassId, int x, int y, int sectionId)
{
	LPEXPLOSION explosion = new CExplosion(explosionClassId, x, y, sectionId);
	CreateObject(explosion);
}

void CGameObjectBehaviour::Explode(LPGAMEOBJECT obj, int explosionClassId, int x, int y)
{
	if (!obj)
		return;

	RemoveObject(obj);
	
	CreateExplosion(explosionClassId, x, y, obj->currentSectionId);
}

void CGameObjectBehaviour::ExplodeAtCenter(LPGAMEOBJECT obj, int explosionClassId)
{
	if (!obj)
		return;

	float objCenterX, objCenterY;
	CalcBoundingBoxCenter(obj, objCenterX, objCenterY);

	// this is to get the position...
	LPEXPLOSION temp = new CExplosion(explosionClassId);
	SetBoundingBoxCenter(temp, objCenterX, objCenterY);
	float x, y;
	temp->GetPosition(x, y);
	delete temp;

	Explode(obj, explosionClassId, x, y);
}

void CGameObjectBehaviour::CreateObject(LPGAMEOBJECT obj)
{
	CCreateObjectEvent* ce = new CCreateObjectEvent(obj);
	CGame::AddGameEvent(ce);
}

void CGameObjectBehaviour::CreateObjectAtCenterOfAnother(LPGAMEOBJECT newObj, LPGAMEOBJECT fromObj)
{
	float centerX, centerY;
	CalcBoundingBoxCenter(fromObj, centerX, centerY);

	SetBoundingBoxCenter(newObj, centerX, centerY);
	newObj->currentSectionId = fromObj->currentSectionId;

	CreateObject(newObj);
}

void CGameObjectBehaviour::RemoveObject(LPGAMEOBJECT obj, bool isDestroyAfterRemove)
{
	CRemoveObjectEvent* re = new CRemoveObjectEvent(obj, isDestroyAfterRemove);
	CGame::AddGameEvent(re);
}

void CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(CBullet* bullet, CEnemy* enemy)
{
	if (bullet->isFriendly)
	{
		enemy->TakeDamage(bullet->damage);
		CGameObjectBehaviour::RemoveObject(bullet);
	}
}
