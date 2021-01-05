#include "Item.h"
#include "GameGlobal.h"
#include "GameObjectBehaviour.h"

CItem::CItem(int classId, int x, int y, int sectionId, bool isFlashy) : CAnimatableObject::CAnimatableObject(classId, x, y, sectionId)
{
	this->isFlashy = isFlashy;
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

	if (
		obj->classId == CLASS_SOPHIA ||
		obj->classId == CLASS_JASONSIDEVIEW ||
		obj->classId == CLASS_JASONOVERHEAD
		)
	{
		ApplyEffect(obj->classId);
		CGameObjectBehaviour::RemoveObject(this);
	}
}

void CItem::HandleOverlap(LPGAMEOBJECT overlappedObj)
{
	if (
		overlappedObj->classId == CLASS_SOPHIA ||
		overlappedObj->classId == CLASS_JASONSIDEVIEW ||
		overlappedObj->classId == CLASS_JASONOVERHEAD
		)
	{
		ApplyEffect(overlappedObj->classId);
		CGameObjectBehaviour::RemoveObject(this);
	}
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
	CAnimatableObject::Update(dt, coObjs);
}

void CItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ITEM_BOUNDBOX_OFFSETX;
	right = left + ITEM_BOUNDBOX_WIDTH;
	top = y + ITEM_BOUNDBOX_OFFSETY;
	bottom = top + ITEM_BOUNDBOX_HEIGHT;
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
