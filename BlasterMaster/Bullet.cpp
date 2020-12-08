#include "Bullet.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

CBullet::CBullet(int x, int y, int sectionId, int animsId, bool isFriendly) : CAnimatableObject::CAnimatableObject(-1, x, y, sectionId, animsId)
{
	this->isFriendly = isFriendly;
}

void CBullet::CalcExplosionCenterPos(float& explosionCenterX, float& explosionCenterY)
{
	CGameObjectBehaviour::CalcBoundingBoxCenter(this, explosionCenterX, explosionCenterY);
}

void CBullet::Explode(LPEXPLOSION explosion)
{
	CRemoveObjectEvent* re = new CRemoveObjectEvent(this);
	CGame::AddGameEvent(re);

	explosion->currentSectionId = this->currentSectionId;
	float explosionCenterX, explosionCenterY;

	CalcExplosionCenterPos(explosionCenterX, explosionCenterY);
	CGameObjectBehaviour::SetBoundingBoxCenter(explosion, explosionCenterX, explosionCenterY);

	float l, t, r, b;
	explosion->GetBoundingBox(l, t, r, b);
	DebugOut(L"wtf is this %f %f %f", l, explosionCenterX, r);

	CCreateObjectEvent* ce = new CCreateObjectEvent(explosion);
	CGame::AddGameEvent(ce);
}


