#include "Item.h"
#include "GameGlobal.h"
#include "GameObjectBehaviour.h"

CItem::CItem(int classId, int x, int y, int sectionId, bool isFlashy) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId)
{
	x = int(x);
	y = int(y);

	this->isFlashy = isFlashy;

	normalPhaseTimer = new CTimer(this, ITEM_NORMAL_PHASE_DURATION, 1);

	blinkingPhaseTimer = new CTimer(this, ITEM_BLINKING_PHASE_DURATION, 1);
	blinkingPhaseTimer->Stop();

	blinkTimer = new CTimer(this, ITEM_BLINK_DURATION);
	blinkTimer->Stop();
}

void CItem::UpdateVelocity(DWORD dt)
{
	vx = vy = 0;
}

void CItem::HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent)
{
	if (coEvent == nullptr)
		return;
	if (coEvent->otherObject == this)
		return;

	LPGAMEOBJECT obj = coEvent->otherObject;
	LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentPlayer();
	if (obj == player)
	{
		ApplyEffect(obj->classId);
		CGameObjectBehaviour::RemoveObject(this);
	}
}

void CItem::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentPlayer();
	if (overlappedObj == player)
	{
		ApplyEffect(overlappedObj->classId);
		CGameObjectBehaviour::RemoveObject(this);
	}
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	normalPhaseTimer->Update(dt);
	blinkingPhaseTimer->Update(dt);
	blinkTimer->Update(dt);

	CAnimatableObject::Update(dt, coObjs);
}

void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ITEM_BOUNDBOX_OFFSETX;
	right = left + ITEM_BOUNDBOX_WIDTH;
	top = y + ITEM_BOUNDBOX_OFFSETY;
	bottom = top + ITEM_BOUNDBOX_HEIGHT;
}

void CItem::HandleTimerTick(CTimer* sender)
{
	if (sender == normalPhaseTimer)
	{
		blinkTimer->Start();
		blinkingPhaseTimer->Start();
	}

	if (sender == blinkTimer)
	{
		modifyA = 255 - modifyA;
	}

	if (sender == blinkingPhaseTimer)
	{
		CGameObjectBehaviour::RemoveObject(this);
	}
}

void CItem::ApplyEffect(int playerClassId)
{
	switch (classId)
	{
	case CLASS_ITEM_POWER:
	case CLASS_ITEM_POWER_O:
	{
		int healthAmount = isFlashy ? FLASHY_HEALTH_AMOUNT : HEALTH_AMOUNT;

		switch(playerClassId)
		{
		case CLASS_SOPHIA: 
			CGameGlobal::GetInstance()->AddToSophiaHealth(healthAmount);
			break;
		case CLASS_JASONSIDEVIEW:
		case CLASS_JASONOVERHEAD:
			CGameGlobal::GetInstance()->AddToJasonHealth(healthAmount);
			break;
		}
		break;
	}

	case CLASS_ITEM_GUN:
	case CLASS_ITEM_GUN_O:
		CGameGlobal::GetInstance()->AddToGunLevel(isFlashy ? FLASHY_GUN_AMOUNT : GUN_AMOUNT);
		break;
	
	case CLASS_ITEM_HOVER:
	case CLASS_ITEM_HOVER_O:
		break;

	case CLASS_ITEM_HOMINGMISSILE:
	case CLASS_ITEM_HOMINGMISSILE_O:
		CGameGlobal::GetInstance()->AddToHomingMissile(WEAPON_AMOUNT);
		break;

	case CLASS_ITEM_MULTIWARHEADMISSILE:
	case CLASS_ITEM_MULTIWARHEADMISSILE_O:
		CGameGlobal::GetInstance()->AddToMultiwarheadMissile(WEAPON_AMOUNT);
		break;

	case CLASS_ITEM_THUNDERBREAK:
	case CLASS_ITEM_THUNDERBREAK_O:
		CGameGlobal::GetInstance()->AddToThunderBreak(WEAPON_AMOUNT);
		break;

	default:
		break;
	}
}
