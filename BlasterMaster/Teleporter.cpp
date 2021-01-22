#include "Teleporter.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"
#include "Bullet_Teleporter.h"
#include "Sound.h"

void CTeleporter::UpdateState()
{

}

void CTeleporter::calcTele() {
	int decision = rand() % 5;
	if (decision == 0) {
		vxTele = RandomFloat(-3, 3);
		vyTele = RandomFloat(-3, 3);
	}
	else {
		float Xplayer, Yplayer, xThis, yThis;
		CGameObjectBehaviour::CalcBoundingBoxCenter(CGame::GetInstance()->GetCurrentPlayer(), Xplayer, Yplayer);
		CGameObjectBehaviour::CalcBoundingBoxCenter(this, xThis, yThis);
		if (xThis > Xplayer)
			vxTele = RandomFloat(-3, 0);
		else vxTele = RandomFloat(0, 3);
		if (yThis > Yplayer)
			vyTele = RandomFloat(-3, 0);
		else vyTele = RandomFloat(0, 3);
	}
}

void CTeleporter::UpdateVelocity(DWORD dt) {
	if (!flagStop) {
		if (flagDoneTeleport % TELEPORT_TIMES == 0) {
			calcTele();
			if (flagDoneTeleport / TELEPORT_TIMES == 3)
				ShootPlayer();
		}
		else if (flagDoneTeleport % TELEPORT_TIMES <= 4) {
			vx = 0;
			vy = 0;
		}
		else if (flagDoneTeleport % 2 == 0) {
			vx = -vxTele;
			vy = -vyTele;
		}
		else {
			vx = vxTele;
			vy = vyTele;
		}
		flagDoneTeleport++;
		flagStop = true;
	}
	else {
		vx = 0;
		vy = 0;
	}
}

CTeleporter::CTeleporter(int classId, int x, int y, int sectionId, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = TELEPORTER_HEALTHPOINT;
	flagStop = true;
	UpdateState();
	SetState(TELEPORTER_STATE_SAFE_IDLE);
	idleTimer = new CTimer(this, IDLE_DURATION+100,1);
	switchTimer = new CTimer(this, IDLE_DURATION, 1);
	teleportTimer = new CTimer(this, TELEPORT_DURATION);
	switchTimer->Stop();
	teleportTimer->Stop();
};

void CTeleporter::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (IsBlockableObject(obj))
	{
	}
}

void CTeleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	idleTimer->Update(dt);
	teleportTimer->Update(dt);
	switchTimer->Update(dt);
	// DebugOut(L"Teleporter position %f %f\n", x, y);
	CEnemy::Update(dt, coObjs);
}

void CTeleporter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + TELEPORTER_BOUNDBOX_OFFSETX;
	right = left + TELEPORTER_BOUNDBOX_WIDTH;
	top = y + TELEPORTER_BOUNDBOX_OFFSETY;
	bottom = top + TELEPORTER_BOUNDBOX_HEIGHT;
}

void CTeleporter::HandleTimerTick(LPTIMER sender)
{
	if (sender == idleTimer)
	{
		Sound::getInstance()->play(TELEPORTER_SWITCH, false, 1);
		SetState(TELEPORTER_STATE_SWITCH_TO_UNSAFE);
		switchTimer->Start();
	}
	if (sender == switchTimer) {
		SetState(TELEPORTER_STATE_UNSAFE_IDLE);
		flagDoneTeleport = 0;
		teleportTimer->Start();
	}
	if (sender == teleportTimer) {
		flagStop = false;
		if (flagDoneTeleport == TELEPORT_TIMES*6) {
			teleportTimer->Stop();
			SetState(TELEPORTER_STATE_SWITCH_TO_SAFE);
			idleTimer->Start();
		}
	}
}

void CTeleporter::ShootPlayer()
{
	Sound::getInstance()->play(ENEMY_OVERHEAD_SHOOT, false, 1);
	float dirX, dirY; // direction to the player
	CGameObjectBehaviour::CalcDirectionToPlayer(this, dirX, dirY);

	float Xplayer, Yplayer;
	CGameObjectBehaviour::CalcBoundingBoxCenter(CGame::GetInstance()->GetCurrentPlayer(), Xplayer, Yplayer);

	CBullet_Teleporter* bullet = new CBullet_Teleporter(0, 0, 0, dirX, dirY);
	CGameObjectBehaviour::CreateObjectAtCenterOfAnother(bullet, this);
}