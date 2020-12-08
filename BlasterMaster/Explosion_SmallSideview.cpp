#include "Explosion_SmallSideview.h"

CExplosion_SmallSideview::CExplosion_SmallSideview(int x, int y, int sectionId) : CExplosion(x, y, sectionId, SMALL_EXPLOSION_SIDEVIEW_ANIMATIONS)
{
	SetState(SMALL_EXPLOSION_STATE);
}

void CExplosion_SmallSideview::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_OFFSETX;
	right = left + EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_WIDTH;
	top = y + EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_OFFSETY;
	bottom = top + EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_HEIGHT;
}
