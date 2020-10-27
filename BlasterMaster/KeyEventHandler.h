#pragma once
#include <Windows.h>
#include <unordered_set>

///	Abstract class to define keyboard event handlers
class CKeyEventHandler
{
public:
	/// <summary>
	/// Handle event when a list of keys is in down state 
	/// You can check if a key is down by calling IsKeyDown function (in Utils.h)
	/// </summary>
	virtual void OnKeyHold() = 0;

	/// <summary>
	/// Handle event when a key is JUST pressed (Down)
	/// </summary>
	/// <param name="KeyCode"></param>
	virtual void OnKeyDown(int KeyCode) = 0;

	/// <summary>
	/// Handle event when a key is JUST released (Up)
	/// </summary>
	/// <param name="KeyCode"></param>
	virtual void OnKeyUp(int KeyCode) = 0;
};
typedef CKeyEventHandler * LPKEYEVENTHANDLER;