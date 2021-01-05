#pragma once
#include "AnimatableObject.h"
class CItem : public CAnimatableObject
{
private:
	const int ITEM_BOUNDBOX_OFFSETX = 4;
	const int ITEM_BOUNDBOX_OFFSETY = 4;
	const int ITEM_BOUNDBOX_WIDTH = 8;
	const int ITEM_BOUNDBOX_HEIGHT = 8;

	const int HEALTH_AMOUNT = 10;
	const int FLASHY_HEALTH_AMOUNT = 40;
	const int GUN_AMOUNT = 1;
	const int FLASHY_GUN_AMOUNT = 4;
	const int WEAPON_AMOUNT = 20;

	bool isFlashy;

public:
	CItem() {};
	CItem(int classId, int x, int y, int sectionId, bool isFlashy = false);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void ApplyEffect(int playerClassId);
};

typedef CItem* LPITEM;
