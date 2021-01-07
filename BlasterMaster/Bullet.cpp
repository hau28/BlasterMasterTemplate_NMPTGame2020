#include "Bullet.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

CBullet::CBullet(int classId, int x, int y, int sectionId, bool isFriendly) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId)
{
	this->isFriendly = isFriendly;
	this->isHiddenByForeground = false;
	this->isUpdatedWhenOffScreen = true;

	SetBoundingBoxInfo(classId);
}

void CBullet::SetBoundingBoxInfo(int classId)
{
	switch (classId)
	{
	case CLASS_SMALL_PINK_BULLET:
		// 6 6 9 9
		boundingBoxOffsetX = 6;
		boundingBoxOffsetY = 6;
		boundingBoxWidth = 4;
		boundingBoxHeight = 4;
		break;

	case CLASS_MEDIUM_PINK_BULLET:
		// 5 5 10 10
		boundingBoxOffsetX = 5;
		boundingBoxOffsetY = 5;
		boundingBoxWidth = 6;
		boundingBoxHeight = 6;
		break;

	case CLASS_LARGE_PINK_BULLET:
	case CLASS_LARGE_GRAY_BULLET:
		// 4 4 11 11
		boundingBoxOffsetX = 4;
		boundingBoxOffsetY = 4;
		boundingBoxWidth = 8;
		boundingBoxHeight = 8;
		break;

	case CLASS_JASON_OVERHEAD_BULLET:
	case CLASS_JASON_OVERHEAD_GRENADE:
		// 8 8 15 15
		boundingBoxOffsetX = 8;
		boundingBoxOffsetY = 8;
		boundingBoxWidth = 8;
		boundingBoxHeight = 8;
		break;

	case CLASS_ENEMY_OVERHEAD_BULLET:
		// 9 8 16 15
		boundingBoxOffsetX = 9;
		boundingBoxOffsetY = 8;
		boundingBoxWidth = 8;
		boundingBoxHeight = 8;
		break;

	default:
		break;
	}
}

void CBullet::CalcExplosionCenterPos(float& explosionCenterX, float& explosionCenterY)
{
	CGameObjectBehaviour::CalcBoundingBoxCenter(this, explosionCenterX, explosionCenterY);
}

void CBullet::Explode(int explosionClassId)
{
	float explosionCenterX, explosionCenterY;
	CalcExplosionCenterPos(explosionCenterX, explosionCenterY);

	// this is to get the position...
	LPEXPLOSION temp = new CExplosion(explosionClassId);
	CGameObjectBehaviour::SetBoundingBoxCenter(temp, explosionCenterX, explosionCenterY);
	float x, y;
	temp->GetPosition(x, y);
	delete temp;

	CGameObjectBehaviour::Explode(this, explosionClassId, x, y);
}

bool CBullet::IsBlockableObject(LPGAMEOBJECT obj)
{
	if (obj->classId == CLASS_TILE_PORTAL || obj->classId == CLASS_TILE_PORTAL_O)
		return true;

	return CAnimatableObject::IsBlockableObject(obj);
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + boundingBoxOffsetX;
	right = left + boundingBoxWidth;
	top = y + boundingBoxOffsetY;
	bottom = top + boundingBoxHeight;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CAnimatableObject::Update(dt, coObjs);

	if (isUpdatedWhenOffScreen)
		if (!checkObjInCamera(this))
			CGameObjectBehaviour::RemoveObject(this);
}


