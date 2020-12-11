#include "Bullet.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

CBullet::CBullet(int x, int y, int sectionId, int animsId, bool isFriendly) : CAnimatableObject::CAnimatableObject(-1, x, y, sectionId, animsId)
{
	this->isFriendly = isFriendly;
	this->isHiddenByForeground = false;
	this->isDestroyedWhenOffScreen = true;
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


