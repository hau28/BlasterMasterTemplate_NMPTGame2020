#pragma once
#include <d3d9.h>
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "Portal.h"
#include "GridObjects.h"

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

	void deleteJasonSideview();
	void pushJasonSideview(float x, float y, int sectionID);

	void deleteJasonOverhead();
	void pushJasonOverhead(float x, float y, int sectionID);

	// Nghia Add and remove object.
	void addObject(LPGAMEOBJECT obj);
	void removeObject(LPGAMEOBJECT obj, bool deleteAfterRemoving);

	// vector<CGameObject*> Objects;
	CGridObjects* gridObjects = nullptr;

	LPPORTAL findScenePortal(int port);
};

typedef CSection* LPSECTION;
