#include "Floater.h"

CFloater::CFloater(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId) 
{
	SetState(FLOATER1_STATE_LIVE);
};

void CFloater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
