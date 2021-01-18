#include "BossArm.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  


CBossArm::CBossArm(int classId, int x, int y, int sectionId, int animsId, int state) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	SetState(state);
	isUpdatedWhenOffScreen = true;
};

void CBossArm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + BOSSARM_BOUNDBOX_OFFSETX;
	right = left + BOSSARM_BOUNDBOX_WIDTH;
	top = y + BOSSARM_BOUNDBOX_OFFSETY;
	bottom = top + BOSSARM_BOUNDBOX_HEIGHT;
}


void CBossArm::UpdateVelocity(DWORD dt)
{

}

void CBossArm::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CBossArm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	UpdatePosition(dt);
}

bool CBossArm::IsBlockableObject(LPGAMEOBJECT obj)
{
	return false;
}
