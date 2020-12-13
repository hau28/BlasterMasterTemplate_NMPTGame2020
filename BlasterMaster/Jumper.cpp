#include "Jumper.h"
#include "TileArea.h"
#include "GameObjectBehaviour.h"

#include "Floater.h"
#include "CreateObjectEvent.h"
#include "RemoveObjectEvent.h"

void CJumper::UpdateState()
{
	if (vx < 0)
		SetState(JUMPER_STATE_WALKLEFT);
	else
		SetState(JUMPER_STATE_WALKRIGHT);
}

CJumper::CJumper(int classId, int x, int y, int sectionId, int initLeft, int animsId) : CEnemy::CEnemy(classId, x, y, sectionId, animsId)
{
	healthPoint = JUMPER_HEALTHPOINT;

	vx = JUMPER_MOVE_SPEED;
	vy = 0;
	vyMax = JUMPER_MAX_SPEED;
	if (initLeft)
		vx = -vx;
	flagOnAir = true;
	lastTimeJump = GetTickCount();
	ground = y;
	numJump = 3;
	UpdateState();
}

void CJumper::UpdateVelocity(DWORD dt)
{
	float Xplayer, Yplayer;
	CGame::GetInstance()->GetCurrentPlayer()->GetPosition(Xplayer, Yplayer);

	if (numJump != 3) {
		if (GetTickCount() - lastTimeJump >= JUMP_PERIOD / 6) {
			vy = -JUMPER_JUMP_FORCE;
			if (x>Xplayer)
				vx = -JUMPER_MOVE_SPEED * 3;
			else
				vx = JUMPER_MOVE_SPEED * 3;
			UpdateState();
			lastTimeJump = GetTickCount();
			// DebugOut(L"jumper jumps\n");
			numJump++;
		}
	}
	else {
		if (GetTickCount() - lastTimeJump >= JUMP_PERIOD) {
			vy = -JUMPER_JUMP_FORCE;
			if (this->state == JUMPER_STATE_WALKLEFT)
				vx = -JUMPER_MOVE_SPEED * 3;
			else if (this->state == JUMPER_STATE_WALKRIGHT)
				vx = JUMPER_MOVE_SPEED * 3;
			lastTimeJump = GetTickCount();
			// DebugOut(L"jumper jumps\n");
			numJump=0;
		}
	}

	if (vy<0 && flagOnAir && ground - y >= 28)
		vy = 0;
	vy += JUMPER_GRAVITY;
	vy = max(-vyMax, vy);
	vy = min(vyMax, vy);
}

void CJumper::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;

	if (dynamic_cast<LPTILE_AREA>(obj))
	{
		LPTILE_AREA tileArea = dynamic_cast<LPTILE_AREA>(obj);
		switch (tileArea->classId)
		{
		case CLASS_TILE_BLOCKABLE:
		case CLASS_TILE_PORTAL:
		{
			CGameObjectBehaviour::BlockObject(dt, coEvent);

			if (coEvent->nx != 0)
			{
				vx = coEvent->nx * JUMPER_MOVE_SPEED;
				UpdateState();
			}

			if (coEvent->ny < 0)
			{
				flagOnAir = false;
			}

			float  l, t, r, b, gl, gt, gr, gb;
			this->GetBoundingBox(l, t, r, b);
			obj->GetBoundingBox(gl, gt, gr, gb);
			if (l <= gl)
				flagOnEdge = -1;
			else if (r >= gr)
				flagOnEdge = 1;
			else
				flagOnEdge = 0;
			// DebugOut(L"Ground %f %f %f %f\n", gl, gt, gr, gb);

			break;
		}
		}
	}
}

void CJumper::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	// Chibi cute
}

void CJumper::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + JUMPER_BOUNDBOX_OFFSETX;
	right = left + JUMPER_BOUNDBOX_WIDTH;
	top = y + JUMPER_BOUNDBOX_OFFSETY;
	bottom = top + JUMPER_BOUNDBOX_HEIGHT;
}


void CJumper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs) {
	flagOnAir = true;
	UpdateVelocity(dt);
	ResolveInteractions(dt, coObjs);
	if (numJump<3 && !flagOnAir)
		vx = 0;
	if (!flagOnAir) {
		ground = y;
		if (vx < 0)
			vx = -JUMPER_MOVE_SPEED;
		if (vx > 0)
			vx = JUMPER_MOVE_SPEED;
	}
	if (!flagOnAir && flagOnEdge < 0 && vx < 0) {
		vx = JUMPER_MOVE_SPEED;
		UpdateState();
	}
	else if (!flagOnAir && flagOnEdge > 0 && vx > 0) {
		vx = -JUMPER_MOVE_SPEED;
		UpdateState();
	}
	UpdatePosition(dt);	

	// CuteTN
	flashingEffect->Update(dt);
}