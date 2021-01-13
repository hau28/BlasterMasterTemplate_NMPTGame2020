#include "BreakableBlock.h"
#include "GameObjectBehaviour.h"
#include "Explosion.h"
#include "Bullet.h"
#include "GameGlobal.h"

CBreakableBlock::CBreakableBlock(int classId, int x, int y, int sectionId, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId, animsId)
{
	requireCrusherBeam = classId != CLASS_BREAKABLE_OVERHEAD_BG;
	modifyA = 255;
	isDestroyed = false;
	vx = vy = ax = ay = 0;

	allowOverlapWithBlocks = true;
	isUpdatedWhenOffScreen = true;
	isHiddenByForeground = true;
}

void CBreakableBlock::UpdateVelocity(DWORD dt)
{}

void CBreakableBlock::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;
	if (obj == nullptr)
		return;

	if (isDestroyed)
		return;

	if (dynamic_cast<CBullet*>(obj))
	{
		CBullet* bullet = dynamic_cast<CBullet*>(obj);
		if (bullet->isFriendly)
		{
			if ((!requireCrusherBeam) || (CGameGlobal::GetInstance()->HasCrusherBeam && bullet->classId == CLASS_SOPHIA_BULLET))
			{
				CGameObjectBehaviour::RemoveObject(bullet);
				Explode();
			}
		}
	}
}

void CBreakableBlock::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CBreakableBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (!checkObjInCamera(this))
	{
		isDestroyed = false;
		modifyA = 0;
	}

	CAnimatableObject::Update(dt, coObjs);
}

void CBreakableBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + BREAKABLE_BOUNDBOX_OFFSETX;
	right = left + BREAKABLE_BOUNDBOX_WIDTH;
	top = y + BREAKABLE_BOUNDBOX_OFFSETY;
	bottom = top + BREAKABLE_BOUNDBOX_HEIGHT;
}

void CBreakableBlock::Explode()
{
	CExplosion* explosion = new CExplosion(CLASS_LARGE_EXPLOSION_SIDEVIEW, 0, 0, 0);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(explosion, this);

	this->isDestroyed = true;
	this->modifyA = 255;
}
