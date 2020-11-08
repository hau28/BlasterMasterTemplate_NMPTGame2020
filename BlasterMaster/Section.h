#pragma once
#include <d3dx9.h>
#include <vector>
#include "Game.h"

class CSection
{
private:
	// int sectionId;					we may not need this
	int backgroundTextureId;
	LPSCENE scene;

public:
	CSection() {};
	CSection(LPSCENE scene, int backgroundTextureId);

	void RenderBackground();
	void Update(DWORD dt);
	void Render();

	vector<LPGAMEOBJECT> Objects;
};

typedef CSection* LPSECTION;
