#include "Animations.h"
#include "Utils.h"


#pragma region CAnimation
void CAnimation::AddFrame(int spriteId, DWORD time = 0)
{
	int t = time;
	if (time == 0) 
		t = this->defaultDuration;

	LPSPRITE sprite = CSpriteLib::GetInstance()->Get(spriteId);

	if (sprite == nullptr)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

LPANIMATION_FRAME CAnimation::GetFrame(int frameIndex)
{
	if (frameIndex < 0 || frameIndex >= GetNumberOfFrames())
		return nullptr;

	return frames[frameIndex];
}

void CAnimation::Render(int currentFrameIndex, float x, float y, int alpha)
{
	frames[currentFrameIndex]->GetSprite()->Draw(x, y, alpha);
}


int CAnimation::GetNumberOfFrames()
{
	return frames.size();
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CAnimationHandler
CAnimationHandler::CAnimationHandler(LPANIMATION animation, bool flipX, bool flipY, int timesRotate90)
{ 
	this->animation = animation; 
	this->flipX = flipX;
	this->flipY = flipY;
	this->timesRotate90 = timesRotate90;
	Reset(); 
}

void CAnimationHandler::Render(float x, float y, int alpha)
{
	LPANIMATION_FRAME frame = animation->GetFrame(currentFrameIndex);
	animation->Render(currentFrameIndex, x, y, alpha);
}

void CAnimationHandler::ToNextFrame()
{
	currentFrameIndex = (currentFrameIndex + 1) % animation->GetNumberOfFrames();
}

void CAnimationHandler::Reset()
{
	currentFrameIndex = 0;
	lastFrameTime = GetTickCount();
}

void CAnimationHandler::Update()
{
	DWORD now = GetTickCount();
	DWORD currentFrameDuration = animation->GetFrame(currentFrameIndex)->GetDuration();

	// update when the time lasts long enough...
	if (now - lastFrameTime > currentFrameDuration)
	{
		ToNextFrame();
		lastFrameTime = now;
	}
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CAnimationLib
CAnimationLib* CAnimationLib::__instance = nullptr;

CAnimationLib* CAnimationLib::GetInstance()
{
	if (__instance == nullptr) 
		__instance = new CAnimationLib();

	return __instance;
}

void CAnimationLib::Add(int animationId, LPANIMATION animation)
{
	animations[animationId] = animation;
}

LPANIMATION CAnimationLib::Get(int animationId)
{
	LPANIMATION animation = animations[animationId];

	if (animation == nullptr)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", animationId);

	return animation;
}

void CAnimationLib::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CObjectAnimations
void CObjectAnimations::AddState(int stateId, LPANIMATION animation, bool flipX, bool flipY, int timesRotate90)
{
	animations[stateId] = new CAnimationHandler(animation, flipX, flipY, timesRotate90);
}

LPANIMATION_HANDLER CObjectAnimations::GetState(int stateId)
{
	LPANIMATION_HANDLER ani = animations[stateId];

	if (ani == nullptr)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", stateId);

	return ani;
}

CObjectAnimationHanlders CObjectAnimations::GenerateAnimationHanlders()
{
	unordered_map<int, LPANIMATION_HANDLER> result;
	result.clear();

	for (auto x : animations)
	{
		result[x.first] = new CAnimationHandler(x.second->animation, x.second->flipX, x.second->flipY, x.second->timesRotate90);
	}

	return result;
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CObjectAnimationsLib
CObjectAnimationsLib* CObjectAnimationsLib::__instance = nullptr;

CObjectAnimationsLib* CObjectAnimationsLib::GetInstance()
{
	if (__instance == nullptr) 
		__instance = new CObjectAnimationsLib();
	return __instance;
}

LPObjectAnimations CObjectAnimationsLib::Get(int objAnimsId)
{
	LPObjectAnimations objAnims = objectAnimations[objAnimsId];
	if (objAnims == nullptr)
		DebugOut(L"[ERROR] Failed to find object animations id: %d\n", objAnimsId);
	 
	return objAnims;
}

void CObjectAnimationsLib::Add(int objAnimsId, LPObjectAnimations objectAnimation)
{
	objectAnimations[objAnimsId] = objectAnimation;
}
#pragma endregion
