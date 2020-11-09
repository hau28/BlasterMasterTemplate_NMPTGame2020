#pragma once

#include <Windows.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "Consts.h"
#include "CustomKeyEvent.h"

using namespace std;

void DebugOut(wchar_t *fmt, ...);

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);


/// Check if key is down in the current frame
/// CuteTN Note: Putting IsKeyDown into CGame Class is a really really bad idea.
/// So I provide a shortcut to adapt it :^) which makes things worst... but easier to use
int IsKeyDown(int KeyCode);

/// Get a list of new key events in this frame from CGame.
/// This list is reset updated per frames.
/// Also, keys events are sorted by the time they occur, be sure to iterate them right :)
/// CuteTN Note: Putting NewKeyEvents into CGame Class is a really really bad idea.
/// So I provide a shortcut to adapt it :^) which makes things worst... but easier to use
std::vector<CCustomKeyEvent*> NewKeyEvents();
