#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

// CuteTN Warning: destructors are not yet implemented

#pragma region CAnimationFrame
/// <summary>
/// <para> Contains a sprite and its corresponding duration </para>
/// <para> - spite: the sprite of this frame </para>
/// <para> - time: the duration of this frame </para>
/// </summary>
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD duration;

public:
	CAnimationFrame(LPSPRITE sprite, int duration) { this->sprite = sprite; this->duration = duration; }
	DWORD GetDuration() { return duration; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;
#pragma endregion Contains a sprite and its corresponding duration

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CAnimation
/// <summary>
///	<para> Contains list of frame of an animation </para>
/// <para> - lastFrameTime: no idea :) </para>
/// <para> - defaultDuration: default duration to set if the passed-in frame duration is not positive </para>
/// <para> ---------------------------- </para>
/// <para> old template problem: Every gameobject with the same animation set would point to the same pointer, so they are gonna render the same as each others </para>
/// </summary>
class CAnimation
{
	int defaultDuration;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultDuration = 100) { this->defaultDuration = defaultDuration; }
	void AddFrame(int spriteId, DWORD time);

	/// <summary>
	/// return an animation frame from frame index
	/// </summary>
	/// <param name="frameIndex"></param>
	/// <returns>nullptr if the frameIndex in invalid</returns>
	LPANIMATION_FRAME GetFrame(int frameIndex);

	/// <summary>
	/// <para> Render currentFrame of the animation at (x,y) </para>
	/// <para> NOTE: sometimes Animation object is nullptr ??? HOW ??? </para>
	/// </summary>
	/// <param name="currentFrame"></param>
	/// <param name="x"> x-coordinate, relative to the gameworld (not the camera) </param>
	/// <param name="y"> y-coordinate, relative to the gameworld (not the camera) </param>
	/// <param name="alpha">opacity</param>
	void Render(int currentFrame, float x, float y, int alpha = 255);
	int GetNumberOfFrames();
};

typedef CAnimation *LPANIMATION;
#pragma endregion Contains list of frame of an animation

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CAnimationHandler
/// <summary>
/// <para> Contains an animation and current frame to handle animation logic for a game object </para>
/// </summary>
class CAnimationHandler
{
	DWORD lastFrameTime;
	int currentFrameIndex;


public:
	LPANIMATION animation;
	bool flipX = false;
	bool flipY = false;
	int timesRotate90 = 0;

	CAnimationHandler(LPANIMATION animation, bool flipX = false, bool flipY = false, int timesRotate90 = 0);

	/// <summary>
	/// <para> Render currentFrame of the animation at (x,y) </para>
	/// </summary>
	/// <param name="currentFrame"></param>
	/// <param name="x"> x-coordinate, relative to the gameworld (not the camera) </param>
	/// <param name="y"> y-coordinate, relative to the gameworld (not the camera) </param>
	/// <param name="alpha">opacity</param>
	void Render(float x, float y, int alpha = 255);
	
	void ToNextFrame();

	/// <summary>
	/// Auto update frame based on real time
	/// </summary>
	void Update();
	void Reset();
};

typedef CAnimationHandler* LPANIMATION_HANDLER;
#pragma endregion Contains an animation and current frame and other option to handle animation logic for a game object state

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CAnimationLib
/// <summary>
/// <para> [Flyweight] [Singleton] </para>
/// <para> Contains a list of Animations reference. </para>
/// <para> Only use this to look up Animations and put into an AnimationSet </para>
/// </summary>
class CAnimationLib
{
	static CAnimationLib * __instance;

	/// <summary>
	/// Map from [AnimationId] to [Animation]
	/// </summary>
	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int animationId, LPANIMATION animation);
	LPANIMATION Get(int animationId);
	void Clear();

	static CAnimationLib * GetInstance();
};
#pragma endregion Contains a list of Animations reference

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region AnimationHandlersLib
/// <summary>
/// a map to map from [StateId] to [AnimationHandler]
/// </summary>
typedef unordered_map<int, LPANIMATION_HANDLER> CObjectAnimationHanlders;

/// <summary>
/// Contains a list of animation handler, also have the model of state in there
/// </summary>
class CAnimationHandlersLib
{
	CAnimationHandlersLib() { animHandlers.clear(); };
	static CAnimationHandlersLib* __instance;

	/// <summary>
	/// Map from [state Id] to [animation handler]
	/// </summary>
	CObjectAnimationHanlders animHandlers;

public:
	void Add(int stateId, LPANIMATION_HANDLER animHandler);
	void Add(int stateId, LPANIMATION animation, bool flipX = false, bool flipY = false, int timesRotate90 = 0);
	LPANIMATION_HANDLER Get(int stateId);

	static CAnimationHandlersLib* GetInstance();
};
#pragma endregion Contains a list of animation handler, also have the model of state in there

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CObjectAnimations
/// <summary>
/// Contains all object animations of a Game object family
/// </summary>
class CObjectAnimations
{
	/// <summary>
	/// contains all state Id for this Game object
	/// </summary>
	vector<int> stateIds;

public:
	CObjectAnimations() { stateIds.clear(); };
	void AddState(int stateId);

	/// <summary>
	/// Generate a new list of AnimationHandler for a new game object to use
	/// </summary>
	/// <returns> a map to map from [StateId] to [AnimationHandler]</returns>
	CObjectAnimationHanlders GenerateAnimationHanlders();
};

typedef CObjectAnimations* LPObjectAnimations;
#pragma endregion Contains all object animations of a Game object family

////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region CObjectAnimationsLib
/// <summary>
/// <para> [Flyweight] [Singleton] </para>
/// <para> Contains a list of ObjectAnimation reference </para>
/// <para> Use this to look up a list of ObjectAnimations from a GameObject Family Id </para>
/// </summary>
class CObjectAnimationsLib
{
	CObjectAnimationsLib() {};
	static CObjectAnimationsLib* __instance;

	/// <summary>
	/// Map from [GameObjectFamilyId] to [ObjectAnimations]
	/// </summary>
	unordered_map<int, LPObjectAnimations> objectAnimations;

public:
	void Add(int objAnimsId, LPObjectAnimations objectAnimations);
	LPObjectAnimations Get(int objAnimsId);

	static CObjectAnimationsLib* GetInstance();
};
#pragma endregion Contains a list of ObjectAnimation reference 