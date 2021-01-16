#include "Boss.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  

const int BOSS_BOUNDBOX_WIDTH = 36;
const int BOSS_BOUNDBOX_HEIGHT = 16;
const int BOSS_BOUNDBOX_OFFSETX = 16;
const int BOSS_BOUNDBOX_OFFSETY = 36;
const float BOSS_GRAVITY = 0.015f;
const float BOSS_MAX_FALL_SPEED = 0.14f;
const float BOSS_MOVE_SPEED = 0.06f;
const int NUMBEROFNODESARM = 4;
const float OFFSET_X_ARM_NODE = -20;
const float OFFSET_Y_ARM_NODE = 15;

void CBoss::init_ObjectsArm()
{
	ArmLeft.clear();
	ArmRight.clear();

	//init arms left
	for (int i = 0; i < NUMBEROFNODESARM; i++)
	{
		CBossArm* arm = new CBossArm(CLASS_BOSS_ARM, this->x + i*OFFSET_X_ARM_NODE, this->y + (i+1)*OFFSET_Y_ARM_NODE, this->currentSectionId, 
			BOSS_ARM_ANIMATIONS, BOSS_ARM_STATE_LEFT);
		ArmLeft.push_back(arm);
	}

	HandLeft = new CBossHand(CLASS_BOSS_HAND, this->x - 80, this->y + 80, this->currentSectionId, BOSS_HAND_ANIMATIONS, BOSS_HAND_STATE_LEFT);

	for (int i = 0; i < NUMBEROFNODESARM; i++)
	{
		CBossArm* arm = new CBossArm(CLASS_BOSS_ARM, this->x + 50, this->y + (i+1)*20, this->currentSectionId, BOSS_ARM_ANIMATIONS, BOSS_ARM_STATE_RIGHT);
		ArmRight.push_back(arm);
	}
	HandRight = new CBossHand(CLASS_BOSS_HAND, this->x + 50, this->y + 100, this->currentSectionId, BOSS_HAND_ANIMATIONS, BOSS_HAND_STATE_RIGHT);
}

CBoss::CBoss(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	SetState(BOSS_BODY_STATE);
	this->init_ObjectsArm();
};

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + BOSS_BOUNDBOX_OFFSETX;
	right = left + BOSS_BOUNDBOX_WIDTH;
	top = y + BOSS_BOUNDBOX_OFFSETY;
	bottom = top + BOSS_BOUNDBOX_HEIGHT;
}


void CBoss::UpdateVelocity(DWORD dt)
{

}

void CBoss::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
}

void CBoss::Render(float offsetX, float offsetY)
{
	CAnimatableObject::Render(offsetX, offsetY);
	for ( auto arm : ArmLeft)
	{
		arm->Render(offsetX, offsetY);
	}

	for (auto arm : ArmRight)
	{
		arm->Render(offsetX, offsetY);
	}

	HandLeft->Render(offsetX, offsetY);
	HandRight->Render(offsetX, offsetY);
}