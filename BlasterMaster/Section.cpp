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

	gridObjects = new CGridObjects(bgWidth, bgHeight);
}

void CSection::Update(DWORD dt)
{
	// CuteTN to do: filter out the neccessary objects to update

	// collision objects for static object, i.e. tiles
	//vector<LPGAMEOBJECT> coObjs_static;
	//coObjs_static.clear();

	float cx, cy, cw, ch;
	CGame::GetInstance()->GetCamPos(cx, cy);
	cw = CGame::GetInstance()->GetScreenWidth();
	ch = CGame::GetInstance()->GetScreenHeight();

	vector<LPGAMEOBJECT> objects = gridObjects->GetObjectsInArea(cx, cy, cw, ch);
	gridObjects->ClearObjectsInArea(dt, cx, cy, cw, ch);

	vector<LPGAMEOBJECT> movingObjects;
	for (auto obj : objects)
	{
		if(CGameObjectBehaviour::IsMovableObject(obj))
			movingObjects.push_back(obj);
	}

	for (auto obj : objects)
	{
		if (!obj)
			continue;

		if (dynamic_cast<LPTILE_AREA>(obj))
			obj->Update(dt, nullptr);
		else
		{
			if (obj->isUpdatedWhenOffScreen || checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
			{
				if (CGameObjectBehaviour::IsMovableObject(obj))
					obj->Update(dt, &objects);
				else
					obj->Update(dt, &movingObjects);
			}
		}
	}

	for (auto obj : objects)
	{
		if (obj)
			gridObjects->Add(obj);
	}
}

//SANH-CAMERA:
void CSection::Render(float offset_x, float offset_y)
{
	float cx, cy, cw, ch;
	CGame::GetInstance()->GetCamPos(cx, cy);
	cw = CGame::GetInstance()->GetScreenWidth();
	ch = CGame::GetInstance()->GetScreenHeight();

	vector<LPGAMEOBJECT> objects = gridObjects->GetObjectsInArea(cx, cy, cw, ch);

	// CuteTN Note: the order of rendering would be implemented here :)
	RenderTexture(backgroundTextureId, offset_x, offset_y);

	for (auto obj : objects)
	{
		if (checkObjInCamera(obj, SCREEN_EXTEND_OFFSET_DEFAULT))
			if(obj->isHiddenByForeground)
				obj->Render(offset_x, offset_y);
	}

	RenderTexture(foregroundTextureId, offset_x, offset_y);

	for (auto obj : objects)
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
	gridObjects->SeekAndRemove(CSophia::GetInstance(), false);
}

void CSection::pushSophia(float x, float y, int sectionID)
{
	this->deleteSophia();
	CSophia::GetInstance()->SetPosition(x, y);
	CSophia::GetInstance()->currentSectionId = sectionID;
	//Objects.push_back(CSophia::GetInstance());
	gridObjects->Add(CSophia::GetInstance());
}

void CSection::deleteJasonSideview()
{
	gridObjects->SeekAndRemove(CJasonSideview::GetInstance(), false);
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

	//Objects.push_back(CJasonSideview::GetInstance());
	gridObjects->Add(CJasonSideview::GetInstance());
}


void CSection::deleteJasonOverhead()
{
	gridObjects->SeekAndRemove(CJasonOverhead::GetInstance(), false);
}

void CSection::pushJasonOverhead(float x, float y, int sectionID)
{
	this->deleteJasonOverhead();

	//CJasonOverhead::GetInstance()->SetPosition(x, y);
	//CJasonOverhead::GetInstance()->currentSectionId = sectionID;
	CJasonOverhead::InitInstance(x, y, sectionID);

	//Objects.push_back(CJasonOverhead::GetInstance());
	gridObjects->Add(CJasonOverhead::GetInstance());
}
void CSection::addObject(LPGAMEOBJECT obj)
{
	if (!obj)
		return;

	gridObjects->Add(obj);
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

	gridObjects->SeekAndRemove(obj, deleteAfterRemoving);
}

LPPORTAL CSection::findScenePortal(int port)
{
	if (gridObjects == nullptr)
		return nullptr;

	DebugOut(L"\n8374");
	vector<LPGAMEOBJECT> objects = gridObjects->GetAllObjects();

	DebugOut(L"\n123");
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->classId == CLASS_TILE_SCENEPORTAL)
		{
			LPPORTAL portal = dynamic_cast<LPPORTAL>(objects[i]);
			if (portal->port == port)
				return portal;
		}
	}
	DebugOut(L"\n456");
	return nullptr;
}

bool CSection::checkIsHasBossArm()
{
	vector<LPGAMEOBJECT> objects = gridObjects->GetAllObjects();
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->classId == CLASS_BOSS_ARM)
		{
				return true;
		}
	}
	return false;
}