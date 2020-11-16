#include "Animations.h"


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

void CAnimation::Render(int currentFrameIndex, float x, float y, int alpha, bool flipX, int rotate)
{
	frames[currentFrameIndex]->GetSprite()->Draw(x, y, alpha, flipX, rotate);
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
	animation->Render(currentFrameIndex, x, y, alpha, flipX, timesRotate90);
}

void CAnimationHandler::ToNextFrame()
{
	currentFrameIndex = (currentFrameIndex + 1) % animation->GetNumberOfFrames();

	if (currentFrameIndex == 0)
		currentFrameIndex = startLoopIndex;
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

#pragma region AnimationHandlersLib
CAnimationHandlersLib* CAnimationHandlersLib::__instance = nullptr;

void CAnimationHandlersLib::Add(int stateId, LPANIMATION_HANDLER animHandler)
{
	animHandlers[stateId] = animHandler;
}

void CAnimationHandlersLib::Add(int stateId, LPANIMATION animation, bool flipX, bool flipY, int timesRotate90)
{
	LPANIMATION_HANDLER animHandler = new CAnimationHandler(animation, flipX, flipY, timesRotate90);
	Add(stateId, animHandler);
}

LPANIMATION_HANDLER CAnimationHandlersLib::Get(int stateId)
{
	return animHandlers[stateId];
}

CAnimationHandlersLib* CAnimationHandlersLib::GetInstance()
{
	if (__instance == nullptr)
		__instance = new CAnimationHandlersLib();

	return __instance;
}
#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CObjectAnimations
void CObjectAnimations::AddState(int stateId)
{
	stateIds.push_back(stateId);
}

CObjectAnimationHanlders CObjectAnimations::GenerateAnimationHanlders()
{
	unordered_map<int, LPANIMATION_HANDLER> result;
	result.clear();

	for (auto stateId : stateIds)
	{
		LPANIMATION_HANDLER baseAnimHandler = CAnimationHandlersLib::GetInstance()->Get(stateId);
		result[stateId] = new CAnimationHandler(baseAnimHandler->animation, baseAnimHandler->flipX, baseAnimHandler->flipY, baseAnimHandler->timesRotate90);
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

LPOBJECT_ANIMATIONS CObjectAnimationsLib::Get(int objAnimsId)
{
	LPOBJECT_ANIMATIONS objAnims = objectAnimations[objAnimsId];
	if (objAnims == nullptr)
		DebugOut(L"[ERROR] Failed to find object animations id: %d\n", objAnimsId);
	 
	return objAnims;
}

void CObjectAnimationsLib::Add(int objAnimsId, LPOBJECT_ANIMATIONS objectAnimation)
{
	objectAnimations[objAnimsId] = objectAnimation;
}
#pragma endregion
