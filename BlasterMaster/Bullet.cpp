#include "Bullet.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

CBullet::CBullet(int classId, int x, int y, int sectionId, bool isFriendly) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId, ChooseAnimationsId(classId))
{
	this->isFriendly = isFriendly;
	this->isHiddenByForeground = false;
	this->isDestroyedWhenOffScreen = true;

	SetBoundingBoxInfo(classId);
}

int CBullet::ChooseAnimationsId(int classId)
{
	switch (classId)
	{
	case CLASS_SMALL_PINK_BULLET:
		return SMALL_PINK_BULLET_SIDEVIEW_ANIMATIONS;
	case CLASS_MEDIUM_PINK_BULLET:
		return MEDIUM_PINK_BULLET_SIDEVIEW_ANIMATIONS;
	case CLASS_LARGE_PINK_BULLET:
		return LARGE_PINK_BULLET_SIDEVIEW_ANIMATIONS;
	case CLASS_LARGE_GRAY_BULLET:
		return LARGE_GRAY_BULLET_SIDEVIEW_ANIMATIONS;
	case CLASS_SOPHIA_BULLET:
		return BULLET_SOPHIA_SIDEVIEW_ANIMATIONS;

	default:
		DebugOut(L"[ERROR] Bullet Animation Id for class Id %d is not defined", classId);
		return 0;
	}
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

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + boundingBoxOffsetX;
	right = left + boundingBoxWidth;
	top = y + boundingBoxOffsetY;
	bottom = top + boundingBoxHeight;
}


