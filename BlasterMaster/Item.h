#pragma once
#include "AnimatableObject.h"
#include "ITimeTrackable.h"
#include "Timer.h"
#include "ObjectFlashingEffectPlayer.h"

class CItem : public CAnimatableObject, public ITimeTrackable
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
	bool isAutoDisappear;

	const int ITEM_NORMAL_PHASE_DURATION = 10000;
	const int ITEM_BLINKING_PHASE_DURATION = 2000;
	const int ITEM_BLINK_DURATION = 30;

	LPTIMER normalPhaseTimer = nullptr;
	LPTIMER blinkingPhaseTimer = nullptr;
	LPTIMER blinkTimer = nullptr;

	static const int ITEM_FLASHING_COLOR_DURATION = 30;
	static vector<Color> itemFlashingColors;
	CObjectFlashingEffectPlayer* flashingEffectPlayer = nullptr;

public:
	CItem() {};
	CItem(int classId, int x, int y, int sectionId, bool isFlashy = false, bool isAutoDisappear = true);

	virtual void UpdateVelocity(DWORD dt);
	virtual void HandleCollision(DWORD dt, LPCOLLISIONEVENT coEvent);
	virtual void HandleOverlap(LPGAMEOBJECT overlappedObj);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void HandleTimerTick(CTimer* sender);

	void ApplyEffect(int playerClassId);

	virtual ~CItem();
};

typedef CItem* LPITEM;
