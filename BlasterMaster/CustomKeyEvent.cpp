#include "CustomKeyEvent.h"

CCustomKeyEvent::CCustomKeyEvent(int KeyCode, int DXKeyState)
{
	this->KeyCode = KeyCode;
	if ((DXKeyState & 0x80) > 0)
		this->KeyState = CustomKeyState::KEY_JUST_PRESSED;
	else
		this->KeyState = CustomKeyState::KEY_JUST_RELEASED;
}

bool CCustomKeyEvent::IsDown()
{
	return KeyState == CustomKeyState::KEY_JUST_PRESSED;
}


bool CCustomKeyEvent::IsUp()
{
	return KeyState == CustomKeyState::KEY_JUST_RELEASED;
}



