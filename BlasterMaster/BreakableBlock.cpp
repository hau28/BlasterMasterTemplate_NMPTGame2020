#include "BreakableBlock.h"
#include "GameObjectBehaviour.h"
#include "Explosion.h"
#include "Bullet.h"
#include "GameGlobal.h"
#include "Bullet_Sophia.h"

CBreakableBlock::CBreakableBlock(int classId, int x, int y, int sectionId, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId, animsId)
{
	// CuteTN hot fix: Fix off-by-one error from resource :)
	// this->x++;
	// this->y++;

	requireCrusherBeam = classId != CLASS_BREAKABLE_OVERHEAD_BG;
	isDestroyed = false;
	vx = vy = ax = ay = 0;

	allowOverlapWithBlocks = true;
	isUpdatedWhenOffScreen = true;
	isHiddenByForeground = true;

	zIndex = ZINDEX_BREAKABLE_BLOCKS;
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
			if(bullet->classId != CLASS_JASON_OVERHEAD_GRENADE)
				if ((!requireCrusherBeam) || (CGameGlobal::GetInstance()->HasCrusherBeam && dynamic_cast<CBullet_Sophia*>(bullet)))
				{
					CGameObjectBehaviour::RemoveObject(bullet);
					Explode();
				}
		}
	}
}

void CBreakableBlock::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (isDestroyed)
		return;

	if (dynamic_cast<CBullet*>(overlappedObj))
	{
		CBullet* bullet = dynamic_cast<CBullet*>(overlappedObj);
		if (bullet->isFriendly)
		{
			if(bullet->classId != CLASS_JASON_OVERHEAD_GRENADE)
				if ((!requireCrusherBeam) || (CGameGlobal::GetInstance()->HasCrusherBeam && dynamic_cast<CBullet_Sophia*>(bullet)))
				{
					CGameObjectBehaviour::RemoveObject(bullet);
					Explode();
				}
		}
	}
}

void CBreakableBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (!checkObjInCamera(this))
	{
		isDestroyed = false;
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
}

void CBreakableBlock::Render(float offsetX, float offsetY)
{
	if (!animationHandlers[state])
	{
		DebugOut(L"[ERROR] Missing animation handler of state %d\n", state);
	}

	if (IsDestroyed())
	{
		animationHandlers[state]->Render(x + offsetX, y + offsetY, 255, modifyR, modifyG, modifyB);
		animationHandlers[state]->Render(x + 1 + offsetX, y + 1 + offsetY, 255, modifyR, modifyG, modifyB);
	}
}
