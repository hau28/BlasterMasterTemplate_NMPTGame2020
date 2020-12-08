#include "Explosion.h"
#include "RemoveObjectEvent.h"

CExplosion::CExplosion(int x, int y, int sectionId, int animsId) : CAnimatableObject::CAnimatableObject(-1, x, y, sectionId, animsId)
{
	vx = vy = 0;
}

void CExplosion::UpdateVelocity(DWORD dt)
{
}

void CExplosion::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CExplosion::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
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
