#include "EndingScene.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "Utils.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_STATE_ANIMATION	5
#define SCENE_OBJECT_ANIMATION	6
#define MAX_SCENE_LINE 1024
#define WIDTH_FROG 511

#define ID_STATE_FROG 3301
#define ID_STATE_MOUNTAIN 3302
#define ID_STATE_CREDIT 3303

#define ID_ENDING_SCENE 3401
#define ID_MOUNTAIN 3402

CEndingScene::CEndingScene(int id, LPCWSTR filePath, int startupSectionId) : CScene(id, filePath)
{
	CGame::GetInstance()->SetCamPos(0, 0);
	setState(StateEnding::PEACE);
	CGame::GetInstance()->setBackGroundColor(255,255,255);
}

void CEndingScene::setState(StateEnding state)
{
	this->state = state;
	switch (state)
	{
	case StateEnding::PEACE:
		ID_STATE = ID_STATE_FROG;
		break;
	case StateEnding::EARTHQUAKE:
		ID_STATE = ID_STATE_FROG;
		MountainX = 122;
		MountainY = 112;
		break;
	case StateEnding::FILM:
		ID_STATE = ID_STATE_FROG;
		break;
	case StateEnding::CREDIT:
		ID_STATE = ID_STATE_FROG;
		break;
	default:
		ID_STATE = ID_STATE_FROG;
		break;
	}
}

void CEndingScene::HandleKeyEnter()
{
	auto keyEvents = NewKeyEvents();
	for (auto e : keyEvents)
	{
		int keyCode = e->GetKeyCode();
		if (keyCode == DIK_X && e->IsDown())
		{
			switch (state)
			{
			case StateEnding::PEACE:
				setState(StateEnding::EARTHQUAKE);
				break;
			case StateEnding::EARTHQUAKE:
				setState(StateEnding::FILM);
				break;
			case StateEnding::FILM:
				setState(StateEnding::CREDIT);
				break;
			default:
				break;
			}
		}
	}
}

void CEndingScene::Load()
{
	CGame::GetInstance()->SetCamPos(0, 0);
	setState(StateEnding::PEACE);
	CGame::GetInstance()->setBackGroundColor(255, 255, 255);

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

	LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_ENDING_SCENE);
	Film = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_MOUNTAIN);
	Mountain = objAnims->GenerateAnimationHanlders();
}
void CEndingScene::Update(DWORD dt)
{
	HandleKeyEnter();
	
	//earthquake
	if (state == StateEnding::PEACE)
	{
		MountainX = 122;
		MountainY = 112;

		if (Mountain[ID_STATE_MOUNTAIN]->currentFrameIndex == 1)
			setState(StateEnding::EARTHQUAKE);
		Mountain[ID_STATE_MOUNTAIN]->Update();
	}

	if (state == StateEnding::EARTHQUAKE)
	{
		float cx, cy;
		CGame::GetInstance()->GetCamPos(cx, cy);
		
		if (Film[ID_STATE]->currentFrameIndex != flag_EarthQuake)
		{
			flag_EarthQuake = Film[ID_STATE]->currentFrameIndex;
			if (cy == 0) cy = 2;
			else cy = 0;
		}

		CGame::GetInstance()->SetCamPos(cx, cy);
	}

	if (state == StateEnding::FILM)
	{
		float cx, cy;
		CGame::GetInstance()->GetCamPos(cx,cy);
		if (cx + 1 + CGame::GetInstance()->GetScreenWidth() >= WIDTH_FROG)
			cx = WIDTH_FROG - CGame::GetInstance()->GetScreenWidth();
		else cx += 1;
		CGame::GetInstance()->SetCamPos(cx, cy);
	}

	if (state == StateEnding::CREDIT)
	{

	}

	Film[ID_STATE]->Update();

	//Hau test vo day
	
}
void CEndingScene::Render()
{
	if (state == StateEnding::EARTHQUAKE)
	{
		Mountain[ID_STATE_MOUNTAIN]->Render(MountainX, MountainY);
		MountainY += 0.25;
		if (MountainY >= 150)
		{
			setState(StateEnding::FILM);
			float cx, cy;
			CGame::GetInstance()->GetCamPos(cx, cy);
			CGame::GetInstance()->SetCamPos(0, cy);
		}
	}
	if (state == StateEnding::PEACE)
		Mountain[ID_STATE_MOUNTAIN]->Render(MountainX, MountainY);

	Film[ID_STATE]->Render(0, 0);
}
void CEndingScene::Unload()
{
}

void CEndingScene::_ParseSection_TEXTURES(string line)
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
void CEndingScene::_ParseSection_SPRITES(string line)
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
void CEndingScene::_ParseSection_ANIMATIONS(string line)
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
void CEndingScene::_ParseSection_STATE_ANIMATION(string line)
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
void CEndingScene::_ParseSection_OBJECT_ANIMATIONS(string line)
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

