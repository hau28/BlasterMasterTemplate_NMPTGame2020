#pragma once

/// <summary>
/// KeyState but friendlier to use
/// </summary>
enum class CustomKeyState
{
	KEY_JUST_PRESSED,
	KEY_JUST_RELEASED,
};

/// <summary>
/// This store the information of a key event, but friendlier to use
/// </summary>
class CCustomKeyEvent
{
private:
	int KeyCode;
	CustomKeyState KeyState;

public:
	/// <summary>
	/// initialize new key event
	/// </summary>
	/// <param name="KeyCode"></param>
	/// <param name="DXKeyState">DirectX Device KeyState</param>
	CCustomKeyEvent(int KeyCode, int DXKeyState);

	int GetKeyCode() { return KeyCode; };
	CustomKeyState GetKeyState() { return KeyState; };

	/// <summary>
	/// check if this key event is down key event
	/// </summary>
	/// <returns></returns>
	bool IsDown();

	/// <summary>
	/// check if this key event is up key event
	/// </summary>
	/// <returns></returns>
	bool IsUp();
};



