#pragma once
#include <d3d9.h>
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"

class CSection
{
private:
	// int sectionId;					we may not need this
	int backgroundTextureId;
	int foregroundTextureId;
	// LPSCENE scene;					why we need to know its scene anyway?
	int bgWidth, bgHeight;

public:
	CSection() {};
	CSection(int bgTextureId, int fgTextureId);

	int getBgWidth() { return bgWidth; };
	int getBgHeight() { return bgHeight; };
	void Update(DWORD dt);

	//SANH-CAMERA
	void Render(float offset_x = 0, float offset_y = 0);
	void RenderTexture(int textureId, float offset_x = 0, float offset_y = 0);
	void deleteSophia();
	void pushSophia(float x, float y, int sectionID);

	vector<CGameObject*> Objects;
};

typedef CSection* LPSECTION;
