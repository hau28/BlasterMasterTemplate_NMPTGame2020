#include "Boss.h"
#include "TileArea.h"
#include "CollisionSolver.h"
#include "GameObjectBehaviour.h"
#include <stdlib.h>
#include <time.h>  
#include <math.h>
#include "GameObjectBehaviour.h"
#include "JasonOverhead.h"
#include "GameGlobal.h"
#include "Bullet_Boss.h"

const int BOSS_BOUNDBOX_WIDTH = 36;
const int BOSS_BOUNDBOX_HEIGHT = 16;
const int BOSS_BOUNDBOX_OFFSETX = 16;
const int BOSS_BOUNDBOX_OFFSETY = 36;

const float BOSS_MOVE_SPEEDX = 0.06f;
const float BOSS_MOVE_SPEEDY = 0.04f;

const int NUMBEROFNODESARM = 4;
const float OFFSET_X_ARM_NODE = -20;
const float OFFSET_Y_ARM_NODE = 15;

void CBoss::init_RandomTargetLeft()
{
	this->isRandomLocationArmLeft = false;
	
	int rand1 = rand() % 2;
	int rand2 = rand() % 2;
	targeHandtLeftX = - rand1 * 60;
	targetHandLeftY = rand2 * 60;

	while (rand1 == 0 && rand2 == 0)
	{
		int rand1 = rand() % 2;
		int rand2 = rand() % 2;
	}

	rand1 = rand() % 2;
	if (rand1 == 0)
		targetHandLeftY *= -1;

}

void CBoss::init_RandomTargetRight()
{
	this->isRandomLocationArmRight = false;
	int rand1 = rand() % 2;
	int rand2 = rand() % 2;

	while (rand1 == 0 && rand2 == 0)
	{
		int rand1 = rand() % 2;
		int rand2 = rand() % 2;
	}

	targetHandRightX = rand1 * 60;
	targetHandRightY = rand2 * 60;

	rand1 = rand() % 2;
	if (rand1 == 0)
		targetHandRightY *= -1;
}

void CBoss::updateVectorHand()
{
	float xHand, yHand;
	ArmLeft[NUMBEROFNODESARM]->GetPosition(xHand, yHand);
	float dau1 = (targeHandtLeftX + this->x + 15 - xHand);
	float dau2 = (targetHandLeftY + this->y + 20 - yHand);

	float vx = 0.002f, vy = 0.002f;
	if (dau1 < 0) vx *= -1;
	if (dau2 < 0) vy *= -1;

	float hvx, hvy;
	ArmLeft[NUMBEROFNODESARM]->GetSpeed(hvx, hvy);
	ArmLeft[NUMBEROFNODESARM]->SetSpeed(vx + hvx, vy + hvy);

	ArmRight[NUMBEROFNODESARM]->GetPosition(xHand, yHand);
	dau1 = (targetHandRightX + this->x + 15 - xHand);
	dau2 = (targetHandRightY + this->y + 20 - yHand);

	vx = 0.002f, vy = 0.002f;
	if (dau1 < 0) vx *= -1;
	if (dau2 < 0) vy *= -1;

	ArmRight[NUMBEROFNODESARM]->GetSpeed(hvx, hvy);
	ArmRight[NUMBEROFNODESARM]->SetSpeed(vx + hvx, vy + hvy);
}

void CBoss::init_ObjectsArm()
{
	ArmLeft.clear();
	ArmRight.clear();

	//init arms left
	for (int i = 0; i < NUMBEROFNODESARM; i++)
	{
		CBossArm* arm = new CBossArm(CLASS_BOSS_ARM, this->x + i * OFFSET_X_ARM_NODE, this->y + (i + 1) * OFFSET_Y_ARM_NODE, this->currentSectionId,
			BOSS_ARM_ANIMATIONS, BOSS_ARM_STATE_LEFT);

		CGameObjectBehaviour::CreateObject(arm);
		ArmLeft.push_back(arm);
	}

	CBossHand* HandLeft = new CBossHand(CLASS_BOSS_HAND,this->x -80, this->y + 80, this->currentSectionId, BOSS_HAND_ANIMATIONS, BOSS_HAND_STATE_LEFT);
	ArmLeft.push_back(HandLeft);
	CGameObjectBehaviour::CreateObject(HandLeft);

	for (int i = 0; i < NUMBEROFNODESARM; i++)
	{
		CBossArm* arm = new CBossArm(CLASS_BOSS_ARM, this->x + 50, this->y + (i + 1) * 20, this->currentSectionId, BOSS_ARM_ANIMATIONS, BOSS_ARM_STATE_RIGHT);
		ArmRight.push_back(arm);
		CGameObjectBehaviour::CreateObject(arm);
	}
	CBossHand* HandRight = new CBossHand(CLASS_BOSS_HAND, this->x + 50, this->y + 100, this->currentSectionId, BOSS_HAND_ANIMATIONS, BOSS_HAND_STATE_RIGHT);
	ArmRight.push_back(HandRight);
	CGameObjectBehaviour::CreateObject(HandRight);
}

CBoss::CBoss(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	SetState(BOSS_BODY_STATE);
	this->limitTop = 0;
	this->limitRight = 200;
	this->limitLeft = 0;
	this->limitBottom = 80;

	vx = BOSS_MOVE_SPEEDX;
	vy = BOSS_MOVE_SPEEDY;
	int rand1 = rand() % 2;
	int rand2 = rand() % 2;
	if (rand1 == 0) vx *= -1;
	if (rand2 == 0) vy *= -1;

	//init section id boss
	CGameGlobal::GetInstance()->ID_SECTION_BOSSOVERHEAD = sectionId;
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
	if (checkEquals(xLeft, yLeft, targetHandLeftY + this->x + 15, targetHandLeftY + this->y + 20))
		this->isRandomLocationArmLeft = true;

	if (checkEquals(yRight, yRight, targetHandRightX + this->x + 15, targetHandRightY + this->y + 20))
		this->isRandomLocationArmRight = true;
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

void CBoss::updateVectorArmFollowBodyBoss(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	for (auto arm : ArmLeft)
	{
		float avx, avy;
		arm->GetSpeed(avx, avy);
		arm->SetSpeed(avx + this->vx - flagVx, avy + this->vy - flagVy);
	}

	for (auto arm : ArmRight)
	{
		float avx, avy;
		arm->GetSpeed(avx, avy);
		arm->SetSpeed(avx + this->vx - flagVx, avy + this->vy - flagVy);
	}
}

void CBoss::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{

}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	if (isLoadedBossArm == false)
	{
		init_ObjectsArm();
		isLoadedBossArm = true;
	}
	singleShotTimer->Update(dt);
	shootPhaseTimer->Update(dt);

	if (isRandomLocationArmLeft)
		init_RandomTargetLeft();

	if (isRandomLocationArmRight)
		init_RandomTargetRight();

	UpdateVelocity(dt);
	updateVectorArmFollowBodyBoss(dt,coObjs);
	updateVectorHand();
	checkTargetLocation();
	UpdatePosition(dt);
	MoveArmLeft();
	MoveArmRight();
	flagVx = vx;
	flagVy = vy;
}

bool CBoss::IsBlockableObject(LPGAMEOBJECT obj)
{
	return false;
}

void CBoss::MoveArmLeft()
{
		for (int i = NUMBEROFNODESARM - 1; i >= 1; i--)
		{
			//float vx1, vx2, vy1, vy2;
			//ArmLeft[i + 1]->GetSpeed(vx1, vy1);
			//ArmLeft[i - 1]->GetSpeed(vx2, vy2);
			//ArmLeft[i]->SetSpeed((vx1 + vx2) / 2, (vy2 + vy2) / 2);
			float x1, y1, x2, y2, xmid, ymid;
			ArmLeft[i + 1]->GetPosition(x1, y1);
			ArmLeft[i - 1]->GetPosition(x2, y2);
			xmid = (x1 + x2) / 2;
			ymid = (y1 + y2) / 2;

			float curVx, curVy, curX, curY;
			ArmLeft[i]->GetSpeed(curVx, curVy);
			ArmLeft[i]->GetPosition(curX, curY);

			float nVx, nVy;
			CGameObjectBehaviour::NormalizeVector2(xmid - curX, ymid - curY, nVx, nVy);

			curVx += nVx * 0.02;
			curVy += nVy * 0.02;

			curVx *= 0.9; // ma sat
			curVy *= 0.9;

			ArmLeft[i]->SetSpeed(curVx, curVy);
		}

		float vx1, vy1;
		ArmLeft[1]->GetSpeed(vx1, vy1);
		ArmLeft[0]->SetSpeed((vx1 + vx) / 2, (vy1 + vy) / 2);
}

void CBoss::MoveArmRight()
{
		for (int i = NUMBEROFNODESARM - 1; i >= 1; i--)
		{
			float x1, y1, x2, y2, xmid, ymid;
			ArmRight[i + 1]->GetPosition(x1, y1);
			ArmRight[i - 1]->GetPosition(x2, y2);
			xmid = (x1 + x2) / 2;
			ymid = (y1 + y2) / 2;

			float curVx, curVy, curX, curY;
			ArmRight[i]->GetSpeed(curVx, curVy);
			ArmRight[i]->GetPosition(curX, curY);

			float nVx, nVy;
			CGameObjectBehaviour::NormalizeVector2(xmid - curX, ymid - curY, nVx, nVy);

			curVx += nVx * 0.02;
			curVy += nVy * 0.02;

			curVx *= 0.9; // ma sat
			curVy *= 0.9;

			ArmRight[i]->SetSpeed(curVx, curVy);
		}

		float vx1, vy1;
		ArmRight[1]->GetSpeed(vx1, vy1);
		ArmRight[0]->SetSpeed((vx1 + vx) / 2, (vy1 + vy) / 2);
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
}