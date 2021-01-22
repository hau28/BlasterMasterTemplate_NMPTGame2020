#include "EndingScene.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "Utils.h"
#include <stdlib.h>    
#include <time.h>  
#include <algorithm>
#include <string> 
#include "Sound.h"

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
#define ID_STATE_RED 3303
#define ID_STATE_CREDIT 3304
#define ID_STATE_DRAGON 3306
#define ID_STATE_THE_END 3307

#define ID_ENDING_SCENE 3401
#define ID_MOUNTAIN 3402
#define ID_RED_SCENE 3403
#define ID_CREDIT 3404
#define ID_THE_END 3405

CEndingScene::CEndingScene(int id, LPCWSTR filePath, int startupSectionId) : CScene(id, filePath)
{
	CGame::GetInstance()->SetCamPos(0, 0);
	setState(StateEnding::RED);
	CGame::GetInstance()->setBackGroundColor(255,255,255);
}

void CEndingScene::setState(StateEnding state)
{
	this->state = state;
	switch (state)
	{
	case StateEnding::RED:
		MountainX = 122;
		MountainY = 112;
		ID_STATE = ID_STATE_RED;
		break;
	case StateEnding::PEACE:
		MountainY = 0;
		ID_STATE = ID_STATE_FROG;
		break;
	case StateEnding::EARTHQUAKE:	
		Sound::getInstance()->stop(AREA2);
		Sound::getInstance()->play(ENDING, false, 1);
		ID_STATE = ID_STATE_FROG;
		MountainX = 122;
		MountainY = 112;
		break;
	case StateEnding::FILM:
		//SOUND ENDING CAMERA MOVING TO PLAYER AND FROG
		timer = 0;
		ID_STATE = ID_STATE_FROG;
		break;
	case StateEnding::CREDIT:
		Sound::getInstance()->stop(ENDING);
		Sound::getInstance()->play(CREDIT, false, 1);
		CGame::GetInstance()->SetCamPos(0, 0);
		CGame::GetInstance()->setBackGroundColor(0, 0, 0);
		posLetterX = 90;
		posLetterY = 300;
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
	setState(StateEnding::RED);
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

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_RED_SCENE);
	Red = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_CREDIT);
	Credit = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_THE_END);
	TheEnd = objAnims->GenerateAnimationHanlders();

	init_MapLetter();
	init_LetterCredit();
	setState(StateEnding::RED);
}
void CEndingScene::Update(DWORD dt)
{
	if (state == StateEnding::RED)
	{
		Red[ID_STATE_RED]->Update();
		
		MountainX = 122;
		MountainY = 112;

		if (Mountain[ID_STATE_MOUNTAIN]->currentFrameIndex == 1)
			setState(StateEnding::EARTHQUAKE);
		Mountain[ID_STATE_MOUNTAIN]->Update();
	}

	HandleKeyEnter();

	if (state == StateEnding::EARTHQUAKE)
	{
		MountainY += 0.25;
		if (MountainY >= 160)
		{
			setState(StateEnding::PEACE);
			float cx, cy;
			CGame::GetInstance()->GetCamPos(cx, cy);
			CGame::GetInstance()->SetCamPos(0, cy);
		}
	}

	if (state == StateEnding::EARTHQUAKE)
	{
		//so dirty
		if (Film[ID_STATE_FROG]->currentFrameIndex != flag_EarthQuake)
		{
			float cx, cy;
			CGame::GetInstance()->GetCamPos(cx, cy);
			if (cy == 0) cy = 2;
			else cy = 0;
			CGame::GetInstance()->SetCamPos(cx, cy);
			flag_EarthQuake = Film[ID_STATE_FROG]->currentFrameIndex;
		}
		Film[ID_STATE_FROG]->Update();
	}

	if (state == StateEnding::PEACE)
	{
		MountainY += 1;
		if ((int)MountainY % 10 == 0)
		{
			R = rand() % 100 + 156;
			G = rand() % 100 + 156;
			B = rand() % 100 + 156;
		}
		if (MountainY >= 40)
			R = G = B = 255;
		if (MountainY >= 200)
			setState(StateEnding::FILM);
	}

	if (state == StateEnding::FILM)
	{
		float cx, cy;
		timer += 1;
		CGame::GetInstance()->GetCamPos(cx,cy);
		if (cx + 1 + CGame::GetInstance()->GetScreenWidth() >= WIDTH_FROG)
			cx = WIDTH_FROG - CGame::GetInstance()->GetScreenWidth();
		else cx += 1;
		CGame::GetInstance()->SetCamPos(cx, cy);
		Film[ID_STATE_FROG]->Update();

		if (timer >= 500)
			setState(StateEnding::CREDIT);
	}

	if (state == StateEnding::CREDIT)
	{

	}
}
void CEndingScene::Render()
{

	if (state == StateEnding::RED)
	{
		Mountain[ID_STATE_MOUNTAIN]->Render(MountainX, MountainY);
		Red[ID_STATE_RED]->Render(0, 0);
	}

	if (state == StateEnding::EARTHQUAKE)
	{
		Mountain[ID_STATE_MOUNTAIN]->Render(MountainX, MountainY);
		Red[ID_STATE_RED]->Render(0, 0);
	}

	if (state == StateEnding::PEACE)
	{
		Film[ID_STATE_FROG]->Render(0, 0, 255, R, G, B);
	}

	if (state == StateEnding::FILM)
		Film[ID_STATE_FROG]->Render(0, 0);

	if (state == StateEnding::CREDIT)
	{
		move_Camera();
		render_DragonFollowCamera();
		render_LetterCredit();
	}

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

//Tool Credit
void CEndingScene::init_MapLetter() 
{
	mapLetter['!'] = 3310;
	mapLetter['0'] = 3311;
	mapLetter['1'] = 3312;
	mapLetter['2'] = 3313;
	mapLetter['3'] = 3314;
	mapLetter['4'] = 3315;
	mapLetter['5'] = 3316;
	mapLetter['6'] = 3317;
	mapLetter['7'] = 3318;
	mapLetter['8'] = 3319;
	mapLetter['9'] = 3320;
	mapLetter['a'] = 3321;
	mapLetter['b'] = 3322;
	mapLetter['c'] = 3323;
	mapLetter['d'] = 3324;
	mapLetter['e'] = 3325;
	mapLetter['f'] = 3326;
	mapLetter['g'] = 3327;
	mapLetter['h'] = 3328;
	mapLetter['i'] = 3329;
	mapLetter['j'] = 3330;
	mapLetter['k'] = 3331;
	mapLetter['l'] = 3332;
	mapLetter['m'] = 3333;
	mapLetter['n'] = 3334;
	mapLetter['o'] = 3335;
	mapLetter['p'] = 3336;
	mapLetter['q'] = 3337;
	mapLetter['r'] = 3338;
	mapLetter['s'] = 3339;
	mapLetter['t'] = 3340;
	mapLetter['u'] = 3341;
	mapLetter['v'] = 3342;
	mapLetter['w'] = 3343;
	mapLetter['x'] = 3344;
	mapLetter['y'] = 3345;
	mapLetter['z'] = 3346;
}

void CEndingScene::init_LetterCredit()
{
	CGame::GetInstance()->SetCamPos(0, 0);
	posLetterX = 90;
	posLetterY = 200;
	ifstream f;
	f.open(L"CREDIT.txt");

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		add_LineCredit(line);
	}
	
	f.close();

	DebugOut(L"\n[INFO] Done loading CREDIT resources");
}
void CEndingScene::add_LineCredit(string line)
{	
	posLetterX = 70;
	std::transform(line.begin(), line.end(), line.begin(), ::tolower);
	for (int i = 0; i < line.length(); i++)
	{
			char ch = line[i];
			if (ch == ' ')
				posLetterX += 5;
			else
			if (!checkValidLetter(ch))
				posLetterX += 10;
			else
			{
				posLetterX += 10;
				DebugOut(L"\n lx %f ly %f", posLetterX, posLetterY);
				Letter item(posLetterX, posLetterY, ch);
				Paragraph.push_back(item);
			}
	}
	posLetterY += 10;
}

void CEndingScene::render_DragonFollowCamera()
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	Credit[ID_STATE_DRAGON]->Render(cx - 15, cy);
}
void CEndingScene::render_LetterCredit()
{
	for (int i = 0; i < Paragraph.size(); i++)
	{
		Letter item = Paragraph[i];
		int id = mapLetter[item.letter];
		Credit[id]->Render(item.x, item.y);
	}

	TheEnd[ID_STATE_THE_END]->Render(120, posLetterY + 60);
}

void CEndingScene::move_Camera()
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	if (cy >= posLetterY)
		return;
	CGame::GetInstance()->SetCamPos(cx, cy + 0.5);
}

bool CEndingScene::checkValidLetter(char & ch)
{
	if (ch >= 'a' && ch <= 'z')
		return true;
	if (ch >= '0' && ch <= '9')
		return true;
	return false;
}