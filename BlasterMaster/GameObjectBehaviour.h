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
	static void GetBlocked(DWORD dt, LPCOLLISIONEVENT coEvent);
};

