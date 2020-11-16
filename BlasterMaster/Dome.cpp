#include "Dome.h"

CDome::CDome(int classId, int x, int y, int animsId): CEnemy::CEnemy(classId, x, y, animsId) 
{
	SetState(DOME_STATE_UP);
};

void CDome::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + DOME_BOUNDBOX_OFFSETX;
	right = left + DOME_BOUNDBOX_WIDTH;
	top = y + DOME_BOUNDBOX_OFFSETY;
	bottom = top + DOME_BOUNDBOX_HEIGHT;
}
