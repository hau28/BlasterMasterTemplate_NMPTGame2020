#pragma once
#include "Explosion.h"
class CExplosion_SmallSideview : public CExplosion
{
	const int EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_WIDTH = 24;
	const int EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_HEIGHT = 24;
	const int EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_OFFSETX = 0;
	const int EXPLOSION_SMALL_SIDEVIEW_BOUNDBOX_OFFSETY = 0;

public:
	CExplosion_SmallSideview();
	CExplosion_SmallSideview(int x, int y, int sectionId);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

