#include "Section.h"
#include "TileArea.h"
#include "Sophia.h"

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
		if (dynamic_cast<LPTILE_AREA>(obj))
			obj->Update(dt, nullptr);
		else
			obj->Update(dt, &Objects);
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

//SANH-CAMERA:
void CSection::Render(float offset_x, float offset_y)
{
	// CuteTN Note: the order of rendering would be implemented here :)
	RenderBackground(offset_x, offset_y);

	for (auto obj : Objects)
		obj->Render();
}

void CSection::RenderBackground(float offset_x, float offset_y)
{
	LPDIRECT3DTEXTURE9 backgroundTexture = CTextures::GetInstance()->Get(backgroundTextureId);
	CGame::GetInstance()->Draw(offset_x, offset_y, backgroundTexture, 0, 0, bgWidth, bgHeight);
}


//SANH-CAMERA
void CSection::deleteSophia()
{
	int index = -1;
	for (int i=0; i<Objects.size(); i++)
		if (Objects[i]->classId == CLASS_SOPHIA)
		{
			index = i;
			break;
		}
	Objects.erase(Objects.begin()+index);
}

void CSection::pushSophia(float x, float y, int sectionID)
{
	CSophia::GetInstance()->SetPosition(x, y);
	CSophia::GetInstance()->currentSectionId = sectionID;
	Objects.push_back(CSophia::GetInstance());
}