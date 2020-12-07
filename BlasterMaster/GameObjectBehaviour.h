#pragma once
#include "GameObject.h"

/// <summary>
/// A special class to encapsulate some GameObjects common behaviour.
/// So that we dont have to write the same functions again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again and again
/// </summary>
class CGameObjectBehaviour
{
public:
	/// <summary>
	/// <para> Solve the behaviour that an object is blocked another object on a collision event </para>
	/// <para> WARNING: mutate directly on movable object! </para>
	/// <para> coEvent->sourceObj: movable object that get blocked </para>
	/// <para> coEvent->otherObj: blockable object </para>
	/// </summary>
	static void BlockObject(DWORD dt, LPCOLLISIONEVENT coEvent);

	/// <summary>
	/// Calculate the center of a game object
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	static void CalcBoundingBoxCenter(LPGAMEOBJECT obj, float& x, float& y);

	/// <summary>
	/// Set the position of the top-left corner of a gameobject's bounding box
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	static void SetBoundingBoxPos(LPGAMEOBJECT obj, float x, float y);

	static void SetBoundingBoxCenter(LPGAMEOBJECT obj, float x, float y);

	/// <summary>
	/// <para> Calculate the direction from a game object to current player. </para>	
	/// <para> Simply takes the difference between 2 posision vectors </para>
	/// </summary>
	static void CalcDirecttionToPlayer(LPGAMEOBJECT obj, float& x, float& y);

	static float CalcMagnitudeVector2(float x, float y);

	/// <summary>
	/// <para> Normalizes the vector (x,y), then stores the result in the vector (nx,ny) </para>
	/// <para> ie (x,y) and (nx,ny) has the same direction but |(nx,ny)| = 0 </para>
	/// </summary>
	static void NormalizeVector2(float x, float y, float& nx, float& ny);
};

