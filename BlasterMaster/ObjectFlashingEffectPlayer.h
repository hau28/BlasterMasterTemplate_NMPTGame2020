#pragma once
#include "Timer.h"
#include "AnimatableObject.h"

class CObjectFlashingEffectPlayer : ITimeTrackable
{
private:
	CAnimatableObject* hostObject = nullptr;
	vector<Color>* colorScript;
	int currentFrameIndex = 0;

	LPTIMER timer = nullptr;
	void HandleTimerTick(LPTIMER sender);
	void SetColor(Color color);

public:
	CObjectFlashingEffectPlayer() {};
	CObjectFlashingEffectPlayer(CAnimatableObject* hostObj, vector<Color>* colorScript, int frameDuration);

	void Play();
	void Update(int dt);
};

