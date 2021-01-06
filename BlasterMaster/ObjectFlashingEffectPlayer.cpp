#include "ObjectFlashingEffectPlayer.h"

CObjectFlashingEffectPlayer::CObjectFlashingEffectPlayer(CAnimatableObject* hostObj, vector<Color>* colorScript, int frameDuration, bool enableRepeat)
{
	this->hostObject = hostObj;
	this->colorScript = colorScript;

	this->enableRepeat = enableRepeat;

	// why +1? we need a frame to reset modified colors
	if (enableRepeat)
		timer = new CTimer(this, frameDuration);
	else
		timer = new CTimer(this, frameDuration, colorScript->size() + 1);

	timer->Stop();
}

void CObjectFlashingEffectPlayer::Play()
{
	currentFrameIndex = 0;
	timer->Start();
}

void CObjectFlashingEffectPlayer::Update(int dt)
{
	timer->Update(dt);
}

void CObjectFlashingEffectPlayer::HandleTimerTick(LPTIMER sender)
{
	if (currentFrameIndex < colorScript->size())
	{
		SetColor((*colorScript)[currentFrameIndex]);
		currentFrameIndex++;
	}
	else
		if (enableRepeat)
			currentFrameIndex = 0;
		else
			SetColor({ 255, 255, 255 });
}

void CObjectFlashingEffectPlayer::SetColor(Color color)
{
	int r, g, b;
	r = std::get<0>(color);
	g = std::get<1>(color);
	b = std::get<2>(color);

	if (hostObject)
		hostObject->SetModifyColor(r, g, b);
}
