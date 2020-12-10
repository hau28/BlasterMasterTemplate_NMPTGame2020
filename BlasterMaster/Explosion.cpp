#include "Explosion.h"
#include "RemoveObjectEvent.h"

#pragma region init

CExplosion::CExplosion(int classId, int x, int y, int sectionId, int animsId) : CAnimatableObject(classId, x, y, sectionId, classId)
{
}

CExplosion::CExplosion(int classId, int x, int y, int sectionId) : CAnimatableObject(classId, x, y, sectionId, ChooseAnimationsId(classId))
{
	ax = ay = vx = vy = 0;
	int animsId = ChooseAnimationsId(classId);
}

int CExplosion::ChooseAnimationsId(int classId)
{
	switch (classId)
	{
	case CLASS_SMALL_EXPLOSION_SIDEVIEW:
		return SMALL_EXPLOSION_SIDEVIEW_ANIMATIONS;
	case CLASS_LARGE_EXPLOSION_SIDEVIEW:
		return LARGE_EXPLOSION_SIDEVIEW_ANIMATIONS;
	default:
		DebugOut(L"[ERROR] Explosion Animation Id for class Id %d is not defined", classId);
		return 0;
	}
}

#pragma endregion

void CExplosion::UpdateVelocity(DWORD dt)
{
}

void CExplosion::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CExplosion::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
}

void CExplosion::ResolveInteractions(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	vector<LPCOLLISIONEVENT>* colEvents = new vector<LPCOLLISIONEVENT>();

	CheckCollision(dt, coObjs, *colEvents);
	HandleCollisions(dt, colEvents);

	vector<LPGAMEOBJECT>* overlappedObjs = new vector<LPGAMEOBJECT>();
	CheckOverlaps(coObjs, *overlappedObjs);
	HandleOverlaps(overlappedObjs);
}

void CExplosion::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + EXPLOSION_BOUNDBOX_OFFSETX;
	right = left + EXPLOSION_BOUNDBOX_WIDTH;
	top = y + EXPLOSION_BOUNDBOX_OFFSETY;
	bottom = top + EXPLOSION_BOUNDBOX_HEIGHT;
}

void CExplosion::Render(float offsetX, float offsetY)
{
	animationHandlers[state]->Render(x + offsetX, y + offsetY, 255, modifyR, modifyG, modifyB);
	animationHandlers[state]->Update();

	// CuteTN Note: This is a dirty way. But this class wont hurt I think :)
	// auto dispose after done its animation
	if (animationHandlers[state]->animation->GetNumberOfFrames() - 1 == animationHandlers[state]->currentFrameIndex)
	{
		CRemoveObjectEvent* e = new CRemoveObjectEvent(this);
		CGame::AddGameEvent(e);
	}
}
