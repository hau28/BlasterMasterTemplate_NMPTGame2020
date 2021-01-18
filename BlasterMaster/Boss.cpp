#include "Boss.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  
#include <math.h>
#include "Bullet_Boss.h"

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

const float OFFSET_X_RANDOM = 80;
const float OFFSET_Y_RANDOM = 100;

void CBoss::init_RandomTargetLeft()
{
	this->isRandomLocationArmLeft = false;
	int rand1 = rand() % 2;
	int rand2 = rand() % 2;
	targeHandtLeftX = - rand1 * 80;
	targetHandLeftY = rand2 * 80;

	rand1 = rand() % 2;
	if (rand1 == 0)
		targetHandLeftY *= -1;

	float xHand, yHand;
	ArmLeft[NUMBEROFNODESARM]->GetPosition(xHand, yHand);
	float vx = (targeHandtLeftX - xHand) / 1000;
	float vy = (targetHandLeftY - yHand) / 1000;
	ArmLeft[NUMBEROFNODESARM]->SetSpeed(vx,vy);
}
void CBoss::init_RandomTargetRight()
{
	this->isRandomLocationArmRight = false;
	int rand1 = rand() % 2;
	int rand2 = rand() % 2;
	targetHandRightX = rand1 * 80;
	targetHandRightY = rand2 * 80;

	rand1 = rand() % 2;
	if (rand1 == 0)
		targetHandRightY *= -1;

	float xHand, yHand;
	ArmRight[NUMBEROFNODESARM]->GetPosition(xHand, yHand);
	float vx = (targetHandRightX - xHand) / 1000;
	float vy = (targetHandRightY - yHand) / 1000;
	ArmRight[NUMBEROFNODESARM]->SetSpeed(vx, vy);
}

void CBoss::init_ObjectsArm()
{
	ArmLeft.clear();
	ArmRight.clear();

	//init arms left
	for (int i = 0; i < NUMBEROFNODESARM; i++)
	{
		CBossArm* arm = new CBossArm(CLASS_BOSS_ARM, i * OFFSET_X_ARM_NODE, (i + 1) * OFFSET_Y_ARM_NODE, this->currentSectionId,
			BOSS_ARM_ANIMATIONS, BOSS_ARM_STATE_LEFT);
		ArmLeft.push_back(arm);
	}

	CBossHand* HandLeft = new CBossHand(CLASS_BOSS_HAND, -80, 80, this->currentSectionId, BOSS_HAND_ANIMATIONS, BOSS_HAND_STATE_LEFT);
	ArmLeft.push_back(HandLeft);

	for (int i = 0; i < NUMBEROFNODESARM; i++)
	{
		CBossArm* arm = new CBossArm(CLASS_BOSS_ARM, 50, (i + 1) * 20, this->currentSectionId, BOSS_ARM_ANIMATIONS, BOSS_ARM_STATE_RIGHT);
		ArmRight.push_back(arm);
	}
	CBossHand* HandRight = new CBossHand(CLASS_BOSS_HAND, 50, 100, this->currentSectionId, BOSS_HAND_ANIMATIONS, BOSS_HAND_STATE_RIGHT);
	ArmRight.push_back(HandRight);
}

CBoss::CBoss(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	SetState(BOSS_BODY_STATE);
	this->init_ObjectsArm();
	this->limitTop = 0;
	this->limitRight = 200;
	this->limitLeft = 0;
	this->limitBottom = 80;

	vx = 0.02f;
	vy = 0.02f;
	int rand1 = rand() % 2;
	int rand2 = rand() % 2;
	if (rand1 == 0) vx *= -1;
	if (rand2 == 0) vy *= -1;

	singleShotTimer = new CTimer(this, DELAY_BETWEEN_SHOTS, SHOT_PER_SHOOTING_PHASE);
	shootPhaseTimer = new CTimer(this, DELAY_BETWEEN_SHOOTING_PHASES);
};

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + BOSS_BOUNDBOX_OFFSETX;
	right = left + BOSS_BOUNDBOX_WIDTH;
	top = y + BOSS_BOUNDBOX_OFFSETY;
	bottom = top + BOSS_BOUNDBOX_HEIGHT;
}

bool checkEquals(float x1, float y1, float x2, float y2)
{
	if (abs(x1 - x2) > 3)
		return false;
	if (abs(y1 - y2) > 3)
		return false;
	return true;
}
void CBoss::checkTargetLocation()
{
	float xLeft, yLeft, xRight, yRight;

	ArmLeft[NUMBEROFNODESARM]->GetPosition(xLeft, yLeft);
	ArmRight[NUMBEROFNODESARM]->GetPosition(yRight, yRight);
	if (checkEquals(xLeft, yLeft, targetHandLeftY, targetHandRightY))
		this->isRandomLocationArmLeft = true;

	if (checkEquals(xLeft, yLeft, targetHandLeftY, targetHandRightY))
		this->isRandomLocationArmLeft = true;
}

void CBoss::ShootPlayer()
{

	float dirX, dirY; // direction to the player
	CGameObjectBehaviour::CalcDirectionToPlayer(this, dirX, dirY);
	CBullet_Boss* bullet = new CBullet_Boss(0, 0, 0, dirX, dirY);
	CGameObjectBehaviour::CreateObjectAtCenterOfBoss(bullet, this);
}

void CBoss::UpdateVelocity(DWORD dt)
{
	if (y <= this->limitTop || y >= this->limitBottom)
		vy *= -1;

	if (x <= this->limitLeft || x >= this->limitRight)
		vx *= -1;
}

void CBoss::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{

}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	singleShotTimer->Update(dt);
	shootPhaseTimer->Update(dt);

	if (isRandomLocationArmLeft)
		init_RandomTargetLeft();

	if (isRandomLocationArmRight)
		init_RandomTargetRight();

	UpdateVelocity(dt);
	UpdatePosition(dt);
}

bool CBoss::IsBlockableObject(LPGAMEOBJECT obj)
{
	return false;
}

void CBoss::HandleTimerTick(LPTIMER sender)
{
	if (sender == singleShotTimer)
	{
		ShootPlayer();
	}

	if (sender == shootPhaseTimer)
	{
		singleShotTimer->Start();
	}
}

void CBoss::CalcBoundingBoxCenter(LPGAMEOBJECT obj, float& x, float& y)
{

}

void CBoss::Render(float offsetX, float offsetY)
{
	CAnimatableObject::Render(offsetX, offsetY);
	for ( auto arm : ArmLeft)
	{
		arm->Render(offsetX + this->x, offsetY + this->y);
	}

	for (auto arm : ArmRight)
	{
		arm->Render(offsetX + this->x, offsetY + this->y);
	}
}