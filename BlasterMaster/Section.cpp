#include "Section.h"
#include "Textures.h"

CSection::CSection(LPSCENE scene, int backgroundTextureId)
{
	this->scene = scene;
	this->backgroundTextureId = backgroundTextureId;
}

void CSection::Update(DWORD dt)
{
	// CuteTN to do: filter out the neccessary objects to update

	for (auto obj : Objects)
		obj->Update(dt);

	// CutetN todo: set new position for camera
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
	// getting the size of texture
	D3DSURFACE_DESC surfaceDesc;
	backgroundTexture->GetLevelDesc(0, &surfaceDesc);

	CGame::GetInstance()->Draw(0, 0, backgroundTexture, 0, 0, surfaceDesc.Width, surfaceDesc.Height);
}
