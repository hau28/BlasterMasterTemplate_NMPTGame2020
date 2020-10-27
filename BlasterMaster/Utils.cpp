#include <Windows.h>
#include "Utils.h"
#include "Game.h"

void DebugOut(wchar_t *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	wchar_t dbg_out[4096];
	vswprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugString(dbg_out);
}

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);

	// delete wcstring   // << can I ? 
	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring *w = new wstring(wcstring);

	// delete wcstring   // << can I ? 
	return w->c_str();
}

/// Check if key is down in the current frame
/// ---
/// CuteTN Note: Putting IsKeyDown into CGame Class is a really really bad idea.
/// So I provide a shortcut to adapt it :^) which makes things worst... but easier to use
int IsKeyDown(int KeyCode)
{
	auto game = CGame::GetInstance();
	return game->IsKeyDown(KeyCode);
}

/// <para> Get a list of new key events in this frame from CGame.</para>
/// This list is reset updated per frames.
/// Also, keys events are sorted by the time they occur, be sure to iterate them right :)
/// CuteTN Note: Putting NewKeyEvents into CGame Class is a really really bad idea.
/// So I provide a shortcut to adapt it :^) which makes things worst... but easier to use
vector<CCustomKeyEvent*> NewKeyEvents()
{
	auto game = CGame::GetInstance();
	return game->CustomKeyEvents;
}
