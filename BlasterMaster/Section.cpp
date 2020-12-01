#include "Section.h"
#include "TileArea.h"
#include "Sophia.h"
#include "CollisionSolver.h"

CSection::CSection(int bgTextureId, int fgTextureId)
{
	this->backgroundTextureId = bgTextureId;
	this->foregroundTextureId = fgTextureId;

	LPDIRECT3DTEXTURE9 backgroundTexture = CTextures::GetInstance()->Get(backgroundTextureId);

	// getting the size of texture
	D3DSURFACE_DESC surfaceDesc;
	backgroundTexture->GetLevelDesc(0, &surfaceDesc);

	bgWidth = surfaceDesc.Width;
	bgHeight = surfaceDesc.Height;
}

bool checkObjInCamera(LPGAMEOBJECT obj)
{
	if (obj->classId == CLASS_SOPHIA || obj->classId == CLASS_JASONSIDEVIEW)
		return true; // Sanh Fix bug section switch

	//Point A is point left top 
	//Point B is point right bottom
	float Ax, Bx, Ay, By;
	CGame::GetInstance()->GetCamPos(Ax, Ay);

	Bx = Ax + CGame::GetInstance()->GetScreenWidth();
	By = Ay + CGame::GetInstance()->GetScreenHeight();

	float oL, oR, oT, oB;
	float tL, tT, tR, tB;
	obj->GetBoundingBox(oL, oT, oR, oB);
	return CCollisionSolver::IsOverlapped(Ax, Ay, Bx, By, oL, oT, oR, oB, tL, tT, tR, tB);
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
		{
			if (checkObjInCamera(obj))
			obj->Update(dt, &Objects);
		}
	}
}

void CSection::Render()
{
	// CuteTN Note: the order of rendering would be implemented here :)
	RenderTexture(backgroundTextureId);

	for (auto obj : Objects)
		obj->Render();

	RenderTexture(foregroundTextureId);
}

void CSection::RenderTexture(int textureId)
{
	LPDIRECT3DTEXTURE9 backgroundTexture = CTextures::GetInstance()->Get(textureId);
	CGame::GetInstance()->Draw(0, 0, backgroundTexture, 0, 0, bgWidth, bgHeight);
}

//SANH-CAMERA:
void CSection::Render(float offset_x, float offset_y)
{
	// CuteTN Note: the order of rendering would be implemented here :)
	RenderTexture(backgroundTextureId, offset_x, offset_y);

	for (auto obj : Objects)
		obj->Render();

	RenderTexture(foregroundTextureId, offset_x, offset_y);
}

void CSection::RenderTexture(int textureId, float offset_x, float offset_y)
{
	LPDIRECT3DTEXTURE9 backgroundTexture = CTextures::GetInstance()->Get(textureId);
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