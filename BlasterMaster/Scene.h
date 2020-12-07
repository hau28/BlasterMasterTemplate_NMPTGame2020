#pragma once

#include <d3dx9.h>
#include <map>
#include <string>
#include "Utils.h"

using namespace std;

class CScene
{
protected:
	int id;
	LPCWSTR sceneFilePath;

public: 
	CScene() {};
	CScene(int id, LPCWSTR filePath);

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};

typedef CScene * LPSCENE;
