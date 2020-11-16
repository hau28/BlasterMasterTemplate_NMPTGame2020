#include "Section.h"
#include "TileArea.h"

CSection::CSection(int bgTextureId)
{
	this->backgroundTextureId = bgTextureId;

	LPDIRECT3DTEXTURE9 backgroundTexture = CTextures::GetInstance()->Get(backgroundTextureId);

	// getting the size of texture
	D3DSURFACE_DESC surfaceDesc;
	backgroundTexture->GetLevelDesc(0, &surfaceDesc);

	bgWidth = surfaceDesc.Width;
	bgHeight = surfaceDesc.Height;
}

void CSection::Update(DWORD dt)
{
	// CuteTN to do: filter out the neccessary objects to update

	// collision objects for static object, i.e. tiles
	//vector<LPGAMEOBJECT> coObjs_static;
	//coObjs_static.clear();
	int cnt = 0;

	for (auto obj : Objects)
	{
		DebugOut(L"oh i oh i%d\n", ++cnt);
		if (dynamic_cast<LPTILE_AREA>(obj))
			obj->Update(dt, nullptr);
		else
			obj->Update(dt, &Objects);
		DebugOut(L"oh i oh i%d\n", cnt);
	}
}

void CSection::Render()
{
	// CuteTN Note: the order of rendering would be implemented here :)
	RenderBackground();

	for (auto obj : Objects)
		obj->Render();
}

void CSection::RenderBackground()
{
	LPDIRECT3DTEXTURE9 backgroundTexture = CTextures::GetInstance()->Get(backgroundTextureId);
	CGame::GetInstance()->Draw(0, 0, backgroundTexture, 0, 0, bgWidth, bgHeight);
}
