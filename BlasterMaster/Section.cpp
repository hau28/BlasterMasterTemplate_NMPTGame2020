#include "Section.h"
#include "TileArea.h"
#include "Sophia.h"
#include "CollisionSolver.h"
#include "JasonSideview.h"
#include "GameObjectBehaviour.h"
#include "JasonOverhead.h"
#include "GameGlobal.h"
#include <stdlib.h>    
#include <time.h>  

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

void CSection::Update(DWORD dt)
{
	// CuteTN to do: filter out the neccessary objects to update

	// collision objects for static object, i.e. tiles
	//vector<LPGAMEOBJECT> coObjs_static;
	//coObjs_static.clear();
	int cnt = 0;

	for (auto obj : Objects)
	{
		if (!obj)
			continue;

		if (dynamic_cast<LPTILE_AREA>(obj))
			obj->Update(dt, nullptr);
		else
		{
			if (obj->isUpdatedWhenOffScreen || checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
			{
				obj->Update(dt, &Objects);
			}
		}
	}
}

//SANH-CAMERA:
void CSection::Render(float offset_x, float offset_y)
{
	// CuteTN Note: the order of rendering would be implemented here :)
	RenderTexture(backgroundTextureId, offset_x, offset_y);

	//if (GameState::PLAY_OVERHEAD == CGame::GetInstance()->GetState())
	//{
	//	CJasonOverhead::GetInstance()->SetPosition(60, 60);
	//	CJasonOverhead::GetInstance()->Render(0, 0);
	//}

	for (auto obj : Objects)
	{
		if (checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
			if(obj->isHiddenByForeground)
				obj->Render(offset_x, offset_y);
	}

	RenderTexture(foregroundTextureId, offset_x, offset_y);

	for (auto obj : Objects)
	{
		if (checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
			if (!obj->isHiddenByForeground)
				obj->Render(offset_x, offset_y);
	}
}

void CSection::RenderTexture(int textureId, float offset_x, float offset_y)
{
	LPDIRECT3DTEXTURE9 backgroundTexture = CTextures::GetInstance()->Get(textureId);

	//EFFECT BOSS - SANHLIKE CUTE
	CGameGlobal * global = CGameGlobal::GetInstance();
	if (global->stateBossBlackBackground)
	{
		CGame::GetInstance()->setBackGroundColor(0, 0, 0);
		return;
	}
	if (global->isEffectBoss)
	{
		int R = rand() % 255;
		int G = rand() % 255;
		int B = rand() % 255;
		int A = 255;
		CGame::GetInstance()->Draw(offset_x, offset_y, backgroundTexture, 0, 0, bgWidth, bgHeight, A,false,0,0,0,R,G,B);
	}
	else
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
	if (index == -1) return;
	Objects.erase(Objects.begin()+index);
}

void CSection::pushSophia(float x, float y, int sectionID)
{
	this->deleteSophia();
	CSophia::GetInstance()->SetPosition(x, y);
	CSophia::GetInstance()->currentSectionId = sectionID;
	Objects.push_back(CSophia::GetInstance());
}

void CSection::deleteJasonSideview()
{
	int index = -1;
	for (int i = 0; i < Objects.size(); i++)
		if (Objects[i]->classId == CLASS_JASONSIDEVIEW)
		{
			index = i;
			break;
		}
	if (index == -1)
		return;
	Objects.erase(Objects.begin() + index);
}

void CSection::pushJasonSideview(float x, float y, int sectionID)
{
	this->deleteJasonSideview();
	//CJasonSideview::GetInstance()->Init();
	//CJasonSideview::GetInstance()->yStartFalling = y;
	//CJasonSideview::GetInstance()->yEndFalling = y;
	CJasonSideview::GetInstance()->SetPosition(x, y);
	CJasonSideview::GetInstance()->currentSectionId = sectionID;
	CJasonSideview::InitInstance(x, y, sectionID);
	Objects.push_back(CJasonSideview::GetInstance());
}


void CSection::deleteJasonOverhead()
{
	int index = -1;
	for (int i = 0; i < Objects.size(); i++)
		if (Objects[i]->classId == CLASS_JASONOVERHEAD)
		{
			index = i;
			break;
		}
	if (index == -1)
		return;
	Objects.erase(Objects.begin() + index);
}

void CSection::pushJasonOverhead(float x, float y, int sectionID)
{
	this->deleteJasonOverhead();
	//CJasonOverhead::GetInstance()->SetPosition(x, y);
	//CJasonOverhead::GetInstance()->currentSectionId = sectionID;
	CJasonOverhead::InitInstance(x, y, sectionID);
	Objects.push_back(CJasonOverhead::GetInstance());
}
void CSection::addObject(LPGAMEOBJECT obj)
{
	if (!obj)
		return;

	Objects.push_back(obj);
}

/// <summary>
/// CuteTN Note: Copied from Sanh's function :)
/// </summary>
/// <param name="obj"></param>
/// <param name="deleteAfterRemoving"></param>
void CSection::removeObject(LPGAMEOBJECT obj, bool deleteAfterRemoving)
{
	if (!obj)
		return;

	for (int i = 0; i < Objects.size(); i++)
		if (Objects[i] == obj)
		{
			if (deleteAfterRemoving)
			{
				delete Objects[i];
				Objects[i] = nullptr;
			}

			Objects.erase(Objects.begin() + i);
			break;
		}
}

LPPORTAL CSection::findScenePortal(int port)
{
	for (int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i]->classId == CLASS_TILE_SCENEPORTAL)
		{
			LPPORTAL portal = dynamic_cast<LPPORTAL>(Objects[i]);
			if (portal->port == port)
				return portal;
		}
	}

	return nullptr;
}
