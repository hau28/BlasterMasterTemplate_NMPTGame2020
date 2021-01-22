#include "BossArm.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  
#include "GameGlobal.h"


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
	if (flashingEffect)
		flashingEffect->Update(dt);

	CGameGlobal* global = CGameGlobal::GetInstance();
	if (!global->isRenderBoss || global->isWinGame)
		return;
	UpdatePosition(dt);
}

bool CBossArm::IsBlockableObject(LPGAMEOBJECT obj)
{
	return false;
}

void CBossArm::TakeDamage(int damage)
{
}
