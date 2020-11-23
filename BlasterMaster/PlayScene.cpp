#include "PlayScene.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath, int startupSectionId) : CScene(id, filePath)
{
	CurrentSectionId = startupSectionId;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_STATE_ANIMATION	5
#define SCENE_OBJECT_ANIMATION	6
#define SCENE_COLLISION_BOXES	7
#define SCENE_SECTIONS	8
#define SCENE_CLASSES	9
#define SCENE_OBJECTS	10

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_STATE_ANIMATION(string line)
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

void CPlayScene::_ParseSection_OBJECT_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_COLLISION_BOXES(string line)
{
	// CuteTN Note: Empty functions since we no more need to import collision box :)
}

void CPlayScene::_ParseSection_SECTIONS(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int section_ID = atoi(tokens[0].c_str());
	int texture_ID = atoi(tokens[1].c_str());

	LPDIRECT3DTEXTURE9 background = CTextures::GetInstance()->Get(texture_ID);
	if (background == nullptr)
	{
		DebugOut(L"[ERROR] Backgound of section %d not found!\n", texture_ID);
		return;
	}

	// CuteTN To do: may us delete this later?
	// set the first section as startup
	if (CurrentSectionId == -1)
		CurrentSectionId = section_ID;

	//Add section
	this->Sections[section_ID] = new CSection(texture_ID);
}

void CPlayScene::_ParseSection_CLASSES(string line)
{
	//Already has const file
}

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	//Sanh
	vector<string> tokens = split(line);

	if (tokens.size() < 4) return; // skip invalid lines

	int obj_ID = atoi(tokens[0].c_str());
	int class_ID = atoi(tokens[1].c_str());

	//Create list properties 
	map<string, string> Properties;
	for (int i = 2; i < tokens.size(); i += 2)
	{
		string prop_name = tokens[i].c_str();
		string prop_value = tokens[i + 1].c_str();

		Properties[prop_name] = prop_value;
	}

	// Create a new game object
	int sectionId;
	LPGAMEOBJECT obj = CGameObjectFactory::Create(class_ID, Properties, sectionId);
	
	// current player
	if (class_ID == CLASS_SOPHIA)
		this->player = obj;
	
	// add portal to library
	if (class_ID == CLASS_TILE_PORTAL)
		CPortalLib::GetInstance()->Add(obj_ID, dynamic_cast<LPPORTAL>(obj));

	if (obj == nullptr)
		DebugOut(L"[ERROR] Cannot create object with object Id: %d\n", obj_ID);


	Sections[sectionId]->Objects.push_back(obj);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		// CuteTN Note: I'm gonna end this code's whole career :)

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 	section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") {	section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[STATE_ANIMATIONS]") { section = SCENE_SECTION_STATE_ANIMATION; continue; }
		if (line == "[OBJECT_ANIMATIONS]") { section = SCENE_OBJECT_ANIMATION; continue; }
		if (line == "[COLLISION_BOXES]") { section = SCENE_COLLISION_BOXES; continue;}
		if (line == "[SECTIONS]") { section = SCENE_SECTIONS; continue;}
		if (line == "[CLASSES]") { section = SCENE_CLASSES; continue;}
		if (line == "[OBJECTS]") { section = SCENE_OBJECTS; continue;}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	
		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line);   break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line);break;
			case SCENE_SECTION_STATE_ANIMATION: _ParseSection_STATE_ANIMATION(line);  break;
			case SCENE_OBJECT_ANIMATION: _ParseSection_OBJECT_ANIMATIONS(line);  break;
			case SCENE_COLLISION_BOXES: _ParseSection_COLLISION_BOXES(line);  break;
			case SCENE_SECTIONS: _ParseSection_SECTIONS(line);  break;
			case SCENE_CLASSES: _ParseSection_CLASSES(line); break;
			case SCENE_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	/*
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	// if (player == nullptr) return; 
		*/
	
	//Sanh code 
	// Update camera to follow mario

	LPSECTION section = this->GetCurrentSection();
	float width_section = section->getBgWidth();
	float height_section = section->getBgHeight();
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2 - 12;
	cy -= game->GetScreenHeight() / 2 - 8;

	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT)
	{
		game->GetCamPos(cx, cy);
		if (cx + 1.5 <= width_section)
			cx += 1.5;
		else cx = width_section;
		float x_sophia, y_sophia;
		CSophia::GetInstance()->GetPosition(x_sophia,y_sophia);
		CSophia::GetInstance()->SetPosition(x_sophia + 0.2, y_sophia);
	}

	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT)
	{
		game->GetCamPos(cx, cy);
		if (cx - 1.5 + game->GetScreenWidth() >= 0)
			cx -= 1.5;
		else cx = -game->GetScreenWidth();
		float x_sophia, y_sophia;
		CSophia::GetInstance()->GetPosition(x_sophia, y_sophia);
		CSophia::GetInstance()->SetPosition(x_sophia - 0.2, y_sophia);
	}

	//Need know pos of section background
	if (CGame::GetInstance()->GetState() != GameState::SECTION_SWITCH_LEFT)
		if (cx < 0) cx = 0;
	if (cy < 0) cy = 0;

	//SANH-CAMERA
	if (CGame::GetInstance()->GetState() != GameState::SECTION_SWITCH_RIGHT)
		if (cx + game->GetScreenWidth() > width_section)
			cx = width_section - game->GetScreenWidth();

	if (cy + game->GetScreenHeight() > height_section)
		cy = height_section - game->GetScreenHeight();

	CGame::GetInstance()->SetCamPos(cx,cy);
	
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT)
	if (cx >= width_section)
	{
		CGame::SetState(GameState::PLAY_SIDEVIEW_SOPHIA);
		Sections[CurrentSectionId]->deleteSophia();
		CurrentSectionId = NextSectionId;
		float x_toPortal, y_toPortal;
		toPortal->GetPosition(x_toPortal, y_toPortal);
		Sections[CurrentSectionId]->pushSophia(x_toPortal+5, y_toPortal, CurrentSectionId);
	}

	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT)
		if (cx + game->GetScreenWidth() <= 0)
		{
			DebugOut(L"SET LEFT");
			CGame::SetState(GameState::PLAY_SIDEVIEW_SOPHIA);
			Sections[CurrentSectionId]->deleteSophia();
			CurrentSectionId = NextSectionId;
			float x_toPortal, y_toPortal;
			toPortal->GetPosition(x_toPortal, y_toPortal);
			Sections[CurrentSectionId]->pushSophia(x_toPortal - 5, y_toPortal, CurrentSectionId);
		}
	Sections[CurrentSectionId]->Update(dt);
	// CuteTN to do: switching section here
}


void CPlayScene::Render()
{
	Sections[CurrentSectionId]->Render();
	//SANH-CAMERA
	if (CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_LEFT
		|| CGame::GetInstance()->GetState() == GameState::SECTION_SWITCH_RIGHT)
	{
		Sections[NextSectionId]->Render(this->offset_x_SectionSwitch,this->offset_y_SectionSwitch);
	}
}

void CPlayScene::set_offset(LPPORTAL fromPortal, LPPORTAL toPortal, string direction)
{
	this->toPortal = toPortal;
	float x_from, y_from, x_to, y_to;
	fromPortal->GetPosition(x_from, y_from);
	toPortal->GetPosition(x_to, y_to);

	DebugOut(L"\nx_from : %f, y_from : %f", x_from, y_from);
	if (direction == "right")
	{
		this->offset_y_SectionSwitch = y_from - y_to;
		this->offset_x_SectionSwitch = Sections[CurrentSectionId]->getBgWidth();
	}

	if (direction == "left")
	{
		this->offset_x_SectionSwitch = -Sections[NextSectionId]->getBgWidth();
		this->offset_y_SectionSwitch = y_from - y_to;
	}
}

string get_DirectionSceneSwitch(LPPORTAL portalA, LPPORTAL portalB)
{
	float xA, xB, yA, yB;

	portalA->GetPosition(xA,yA);
	portalB->GetPosition(xB, yB);
	if (xA >= xB) return "right";
	return "left";
}

void CPlayScene::handleGameEvent(LPGAME_EVENT gameEvent)
{
	//SANH-CAMERA
	if (gameEvent->eventName == "WalkInPortalEvent")
	{
		CWalkInPortalEvent* temp = dynamic_cast<CWalkInPortalEvent*>(gameEvent);
		NextSectionId = temp->get_toPortal()->currentSectionId;
		string direct = get_DirectionSceneSwitch(temp->get_fromPortal(), temp->get_toPortal());
		if (direct == "left")
		{
			CGame::SetState(GameState::SECTION_SWITCH_LEFT);
		}
	
		if (direct == "right")
			CGame::SetState(GameState::SECTION_SWITCH_RIGHT);


		set_offset(temp->get_fromPortal(), temp->get_toPortal(), direct);
		DebugOut(L"OFFSET : %f --- %f", this->offset_x_SectionSwitch, this->offset_y_SectionSwitch);
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	/*
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	// player = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
	*/
	
	// CuteTN To do
}
