#include "IntroScene.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "Utils.h"
#include <stdlib.h>    
#include <time.h>  
#include "SwitchSceneEvent.h"
#include "GameEvent.h"
#include "GameGlobal.h"
#include "Sound.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_STATE_ANIMATION	5
#define SCENE_OBJECT_ANIMATION	6
#define MAX_SCENE_LINE 1024

#define ID_STATE_TITLE 2300
#define ID_STATE_FILMINTRO 2301
#define ID_STATE_SOPHIADOWNGROUND 2302

#define ID_STATE_LEFT1 2303
#define ID_STATE_LEFT0	2304
#define ID_STATE_CONTINUE 2306
#define ID_STATE_END 2305

CIntroScene::CIntroScene(int id, LPCWSTR filePath, int startupSectionId) : CScene(id, filePath)
{
	CGame::GetInstance()->SetCamPos(0, 0);
	this->state = ID_STATE_TITLE;
}
void CIntroScene::HandleKeyEnter()
{
	auto keyEvents = NewKeyEvents();
	for (auto e : keyEvents)
	{
		int keyCode = e->GetKeyCode();
		if (e->IsDown())
		{
			switch (state)
			{
			case ID_STATE_TITLE:
				setState(ID_STATE_SOPHIADOWNGROUND);
				break;
			case ID_STATE_FILMINTRO:
				setState(ID_STATE_TITLE);
			default:
				break;
			}
		}
		if ((keyCode == DIK_UP || keyCode == DIK_DOWN)  && e->IsDown())
		{
			switch (state)
			{
			case ID_STATE_TITLE:
				setState(ID_STATE_SOPHIADOWNGROUND);
				break;
			case ID_STATE_FILMINTRO:
				setState(ID_STATE_TITLE);
				break;
			case ID_STATE_CONTINUE:
				setState(ID_STATE_END);
				break;
			case ID_STATE_END:
				setState(ID_STATE_CONTINUE);
				break;

			default:
				break;
			}
		}
		if ((keyCode == DIK_SPACE || keyCode == DIK_RETURN) && e->IsDown())
		{
			switch (state)
			{
			case ID_STATE_CONTINUE:
				pressContinue();
				break;
			case ID_STATE_END:
				pressEnd();
				break;

			default:
				break;
			}
		}
	}
}

void CIntroScene::Load()
{
	DebugOut(L"[INFO] Start loading INTRO SCENE resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[STATE_ANIMATIONS]") { section = SCENE_SECTION_STATE_ANIMATION; continue; }
		if (line == "[OBJECT_ANIMATIONS]") { section = SCENE_OBJECT_ANIMATION; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line);   break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_STATE_ANIMATION: _ParseSection_STATE_ANIMATION(line);  break;
		case SCENE_OBJECT_ANIMATION: _ParseSection_OBJECT_ANIMATIONS(line);  break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading INTRO scene resources %s\n", sceneFilePath);

	LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(2400);
	animationHandlers = objAnims->GenerateAnimationHanlders();

	CGame::GetInstance()->SetCamPos(0, 0);
	this->state = ID_STATE_TITLE;
	isTitleFinished = false;
	isFilmFinished = false;
	isIntroFinished = false;
	isLeftFinished = false;
	CGameGlobal::GetInstance()->subLeft();
	DebugOut(L"\n left = %d", CGameGlobal::GetInstance()->getLeft());
	switch (CGameGlobal::GetInstance()->getLeft())
	{
		case 1 :
			state = ID_STATE_LEFT1;
			break;

		case 0 : 
			state = ID_STATE_LEFT0;
			break;

		case -1 :
			state = ID_STATE_CONTINUE;
			break;

		default:
		break;
	}
}
void CIntroScene:: Update(DWORD dt)
{
	//Nhap nhay background
	int R = rand() % 256;
	int G = rand() % 256;
	int B = rand() % 256;
	CGame::GetInstance()->setBackGroundColor(R, G, B);
 	HandleKeyEnter();
	switch (state)
	{
	case (ID_STATE_TITLE):
		if (animationHandlers[state]->currentFrameIndex == animationHandlers[state]->animation->GetNumberOfFrames()-1)
			isTitleFinished = true;
		isIntroFinished = isFilmFinished = false;
		break;
	case (ID_STATE_FILMINTRO):
		if (animationHandlers[state]->currentFrameIndex == animationHandlers[state]->animation->GetNumberOfFrames()-1)
			isFilmFinished = true;
		isIntroFinished = isTitleFinished = false;
		break;
	case (ID_STATE_SOPHIADOWNGROUND):
		if (animationHandlers[state]->currentFrameIndex == animationHandlers[state]->animation->GetNumberOfFrames()-1)
			isIntroFinished = true;
		if (animationHandlers[state]->currentFrameIndex == animationHandlers[state]->animation->GetNumberOfFrames() - 2)
		{
			Sound::getInstance()->stop(INTRO_MUSIC);
			Sound::getInstance()->stop(SOPHIA_ENTER_MUSIC);
		}
		isTitleFinished = isFilmFinished = false;
		break;
	case (ID_STATE_LEFT1):
		Sound::getInstance()->stop(INTRO_MUSIC);
		Sound::getInstance()->stop(SOPHIA_ENTER_MUSIC);
		if (animationHandlers[state]->currentFrameIndex == animationHandlers[state]->animation->GetNumberOfFrames() - 1)
			isLeftFinished = true;
		isTitleFinished = isFilmFinished = false;
		break;
	case (ID_STATE_LEFT0):
		Sound::getInstance()->stop(INTRO_MUSIC);
		Sound::getInstance()->stop(SOPHIA_ENTER_MUSIC);
		if (animationHandlers[state]->currentFrameIndex == animationHandlers[state]->animation->GetNumberOfFrames() - 1)
			isLeftFinished = true;
		isTitleFinished = isFilmFinished = false;
		break;
	default:
		break;
	}

	animationHandlers[state]->Update();

	switch (state)
	{
	case (ID_STATE_TITLE):
		if (isTitleFinished && animationHandlers[state]->currentFrameIndex == animationHandlers[state]->startLoopIndex)
		{
			setState(ID_STATE_FILMINTRO);
			animationHandlers[state]->Reset();
		}
		isIntroFinished = isTitleFinished = isFilmFinished = false;
		break;
	case (ID_STATE_FILMINTRO):
		if (isFilmFinished && animationHandlers[state]->currentFrameIndex == animationHandlers[state]->startLoopIndex)
		{	
			animationHandlers[state]->Reset();
			setState(ID_STATE_TITLE);
		}
		isIntroFinished = isTitleFinished = isFilmFinished = false;
		break;
	case (ID_STATE_SOPHIADOWNGROUND):
		if (isIntroFinished && animationHandlers[state]->currentFrameIndex == animationHandlers[state]->startLoopIndex)
		{

			animationHandlers[state]->Reset();
			//CGame::GetInstance()->SwitchScene(ID_SCENE_END);
			CGameEvent* event = new SwitchSceneEvent(ID_SCENE_SIDEVIEW);
			CGame::AddGameEvent(event);
		}
		isIntroFinished = isTitleFinished = isFilmFinished = false;
		break;
	case (ID_STATE_LEFT1):
		if (isLeftFinished && animationHandlers[state]->currentFrameIndex == animationHandlers[state]->startLoopIndex)
		{
			animationHandlers[state]->Reset();
			CGameEvent* event = new SwitchSceneEvent(ID_SCENE_SIDEVIEW);
			if (CGameGlobal::GetInstance()->getPlayer() == 3)
				event = new SwitchSceneEvent(ID_SCENE_OVERHEAD);
			CGame::AddGameEvent(event);
		}
		isIntroFinished = isTitleFinished = isFilmFinished = false;
		break;
	case (ID_STATE_LEFT0):
		if (isLeftFinished && animationHandlers[state]->currentFrameIndex == animationHandlers[state]->startLoopIndex)
		{
			animationHandlers[state]->Reset();
			//CGame::GetInstance()->SwitchScene(ID_SCENE_END);
			CGameEvent* event = new SwitchSceneEvent(ID_SCENE_SIDEVIEW);
			if (CGameGlobal::GetInstance()->getPlayer() == 3)
				event = new SwitchSceneEvent(ID_SCENE_OVERHEAD);
			CGame::AddGameEvent(event);
			isIntroFinished = isTitleFinished = isFilmFinished = false;
		}
	default:
		break;
	}
}
void CIntroScene::Render()
{
	animationHandlers[state]->Render(0, 0);
}
void CIntroScene::Unload()
{

}

void CIntroScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	//int R = atoi(tokens[2].c_str());
	//int G = atoi(tokens[3].c_str());
	//int B = atoi(tokens[4].c_str());

	int R = 254;
	int G = 254;
	int B = 254;

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void CIntroScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int texID = atoi(tokens[1].c_str());
	int l = atoi(tokens[2].c_str());
	int t = atoi(tokens[3].c_str());
	int r = atoi(tokens[4].c_str());
	int b = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == nullptr)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSpriteLib::GetInstance()->Add(ID, l, t, r, b, tex);
}
void CIntroScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	////DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int duration = atoi(tokens[i + 1].c_str());

		DebugOut(L"spriteId: %d   duration: %d\n", sprite_id, duration);
		ani->AddFrame(sprite_id, duration);
	}

	CAnimationLib::GetInstance()->Add(ani_id, ani);
}
void CIntroScene::_ParseSection_STATE_ANIMATION(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return;

	int state_id = atoi(tokens[0].c_str());
	int ani_id = atoi(tokens[1].c_str());
	LPANIMATION ani = CAnimationLib::GetInstance()->Get(ani_id);

	int flipX = atoi(tokens[2].c_str());
	int flipY = atoi(tokens[3].c_str());
	int rotate = atoi(tokens[4].c_str());

	// Add to lib
	CAnimationHandlersLib::GetInstance()->Add(state_id, ani, flipX, flipY, rotate);
	DebugOut(L"[INFO] Added StateId %d, animId %d, flipX %d, flipY %d, rotate %d\n", state_id, ani_id, flipX, flipY, rotate);
}
void CIntroScene::_ParseSection_OBJECT_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int objectAni_id = atoi(tokens[0].c_str());
	LPOBJECT_ANIMATIONS objAni = new CObjectAnimations();

	for (int i = 1; i < tokens.size(); i++)
	{
		int stateId = atoi(tokens[i].c_str());
		objAni->AddState(stateId);
	}

	// Add to lib
	CObjectAnimationsLib::GetInstance()->Add(objectAni_id, objAni);
}

//Reset Game
void CIntroScene::pressContinue()
{
	CGameGlobal::GetInstance()->resetGame();
	CGameGlobal::GetInstance()->subLeft();

	setState(ID_STATE_SOPHIADOWNGROUND);
	animationHandlers[state]->currentFrameIndex = animationHandlers[state]->animation->GetNumberOfFrames() - 1;
	isTitleFinished = false;
	isFilmFinished = false;
	isIntroFinished = false;
	isLeftFinished = false;


}

void CIntroScene::pressEnd()
{
	CGameGlobal::GetInstance()->resetGame();
	CGameEvent* event = new SwitchSceneEvent(ID_SCENE_INTRO);
	CGame::AddGameEvent(event);
	isTitleFinished = false;
	isFilmFinished = false;
	isIntroFinished = false;
	isLeftFinished = false;
}

void CIntroScene::setState(int state)
{
	this->state = state;
	//Stop all play sound
	Sound::getInstance()->stop(INTRO_MUSIC);
	Sound::getInstance()->stop(SOPHIA_ENTER_MUSIC);

	//Play new sound
	switch (state)
	{
	case ID_STATE_TITLE:
		break;
	case ID_STATE_FILMINTRO:
		Sound::getInstance()->play(INTRO_MUSIC, false, 1);
		break;
	case ID_STATE_SOPHIADOWNGROUND:
		Sound::getInstance()->play(SOPHIA_ENTER_MUSIC, false, 1);
		break;
	default:
		break;
	}
}