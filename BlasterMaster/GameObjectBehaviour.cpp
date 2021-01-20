#include "GameObjectBehaviour.h"
#include "CollisionSolver.h"
#include "Explosion.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "TileArea.h"
#include "BreakableBlock.h"

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

void CGameObjectBehaviour::CalcDirection(LPGAMEOBJECT fromObj, LPGAMEOBJECT toObj, float& x, float& y)
{
	x = y = 0;

	if (fromObj == nullptr)
		return;
	float xF, yF;
	CalcBoundingBoxCenter(fromObj, xF, yF);

	if (toObj == nullptr)
		return;
	float xT, yT;
	CalcBoundingBoxCenter(toObj, xT, yT);

	x = (xT - xF);
	y = (yT - yF);
}

void CGameObjectBehaviour::CalcDirectionToPlayer(LPGAMEOBJECT obj, float& x, float& y)
{
	LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentPlayer();
	CalcDirection(obj, player, x, y);
}

float CGameObjectBehaviour::CalcMagnitudeVector2(float x, float y)
{
	return sqrt(x * x + y * y);
}

void CGameObjectBehaviour::NormalizeVector2(float x, float y, float& nx, float& ny)
{
	float d = CalcMagnitudeVector2(x, y);
	if (d != 0)
	{
		nx = x / d;
		ny = y / d;
	}
	else
	{
		nx = 1;
		ny = 0;
	}
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

void CGameObjectBehaviour::CreateObjectAtCenterOfBoss(LPGAMEOBJECT newObj, LPGAMEOBJECT fromObj)
{
	const float BOSS_OFFSETX_FROM_CENTER = 15;
	const float BOSS_OFFSETY_FROM_CENTER = 36;

	float centerX, centerY;
	CalcBoundingBoxCenter(fromObj, centerX, centerY);

	centerX -= BOSS_OFFSETX_FROM_CENTER;
	centerY -= BOSS_OFFSETY_FROM_CENTER;

	SetBoundingBoxCenter(newObj, centerX, centerY);
	newObj->currentSectionId = fromObj->currentSectionId;

	CreateObject(newObj);
}

void CGameObjectBehaviour::RemoveObject(LPGAMEOBJECT obj, bool isDestroyAfterRemove)
{
	CRemoveObjectEvent* re = new CRemoveObjectEvent(obj, isDestroyAfterRemove);
	CGame::AddGameEvent(re);
}

void CGameObjectBehaviour::HandleFriendlyBulletHitsEnemy(CBullet* bullet, CEnemy* enemy, bool penetrable)
{
	if (bullet->isFriendly)
	{
		// not allowing an enemy can be damaged by too many bullet :)
		if (enemy->GetHealthPoint() > 0)
		{
			enemy->TakeDamage(bullet->damage);

			// not allowing a bullet can damage too many target :)
			bullet->damage = 0;

			if(!penetrable)
				bullet->Explode(CLASS_SMALL_EXPLOSION_SIDEVIEW);
		}
	}
}

bool CGameObjectBehaviour::IsMovableObject(LPGAMEOBJECT obj)
{
	if(dynamic_cast<CTileArea*>(obj))
		return false;

	if (dynamic_cast<CBreakableBlock*>(obj))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CompareRenderOrderSideview(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2)
{
	return obj1->zIndex < obj2->zIndex;
}

bool CompareRenderOrderOverhead(LPGAMEOBJECT obj1, LPGAMEOBJECT obj2)
{
	if(obj1->zIndex != obj2->zIndex)
		return obj1->zIndex < obj2->zIndex;

	float cx1, cy1, cx2, cy2;
	CGameObjectBehaviour::CalcBoundingBoxCenter(obj1, cx1, cy1);
	CGameObjectBehaviour::CalcBoundingBoxCenter(obj2, cx2, cy2);

	return cy1 < cy2;
}

void MySort(vector<LPGAMEOBJECT>& objects, function<int(LPGAMEOBJECT, LPGAMEOBJECT)> cmp)
{
	// MySort(objects, 0, objects.size()-1, cmp);
	for(int i=0; i<objects.size(); i++)
		for (int j = i + 1; j < objects.size(); j++)
		{
			if (cmp(objects[i], objects[j]))
			{
				LPGAMEOBJECT temp = objects[i];
				objects[i] = objects[j];
				objects[j] = temp;
			}
		}
}

// Quick sort baby: Not work baby
void MySort(vector<LPGAMEOBJECT>& objects, int l, int r, function<int(LPGAMEOBJECT, LPGAMEOBJECT)> cmp)
{
	if (l >= r)
		return;

	int i = l;
	int j = r;
	LPGAMEOBJECT pivotObj = objects[(l + r) / 2];

	while (true)
	{
		while (i <= r && cmp(pivotObj, objects[i]) == -1) i++;
		while (j >= l && cmp(pivotObj, objects[j]) == 1 ) j--;

		if (i < j)
		{
			// swap
			LPGAMEOBJECT temp = objects[i];
			objects[i] = objects[j];
			objects[j] = temp;

			i++;
			j--;
		}
		
		if (i >= j)
			break;
	}

	DebugOut(L"L R %d %d %d %d\n", l, r, i, j);

	MySort(objects, l, j, cmp);
	MySort(objects, i, r, cmp);
}
