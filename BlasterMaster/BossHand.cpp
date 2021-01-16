#include "BossHand.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  


CBossHand::CBossHand(int classId, int x, int y, int sectionId, int animsId, int state) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	SetState(state);
};

void CBossHand::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + BOSSHAND_BOUNDBOX_OFFSETX;
	right = left + BOSSHAND_BOUNDBOX_WIDTH;
	top = y + BOSSHAND_BOUNDBOX_OFFSETY;
	bottom = top + BOSSHAND_BOUNDBOX_HEIGHT;
}

void CBossHand::UpdateVelocity(DWORD dt)
{

}

void CBossHand::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CBossHand::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
}
