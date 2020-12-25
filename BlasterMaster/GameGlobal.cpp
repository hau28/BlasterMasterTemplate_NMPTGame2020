#include "GameGlobal.h"
#include "Sophia.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_STATE_ANIMATION	5
#define SCENE_OBJECT_ANIMATION	6
#define MAX_SCENE_LINE 1024


#define HEALTH 9041
#define HEALTH0 9031
#define HEALTH1 9032
#define HEALTH2 9033
#define HEALTH3 9034
#define HEALTH4 9035
#define HEALTH5 9036
#define HEALTH6 9037
#define HEALTH7 9038
#define HEALTH8 9039

#define X_HEALTH 20
#define Y_HEALTH 120

CGameGlobal* CGameGlobal::_instance = nullptr;

CGameGlobal::CGameGlobal() {
	//init healthSophia
	this->healthSophia = MAX_HEALTH_SOPHIA;
	this->healthJasonSideView = MAX_HEALTH_JASONSIDEVIEW;
	this->healthJasonOverHead = MAX_HEALTH_JASONOVERHEAD;

	//init level Gun
	this->levelGunPower = 0;
	

	ifstream f;
	f.open(L"HEALTHANDGUN.txt");

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
	DebugOut(L"\n[INFO] Done loading Health & Gun scene resources");

	LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(HEALTH);
	HealthPow = objAnims->GenerateAnimationHanlders();
}

CGameGlobal* CGameGlobal::GetInstance()
{
	if (_instance == nullptr) _instance = new CGameGlobal();
	return _instance;
}

void CGameGlobal::getItem(Items item)
{
	int unit_Bar = 0;
	switch (item)
	{
	case Items::PowerGrey:
		break;
	case Items::PowerFlashingRed:
		break;
	case Items::HoverGrey:
		break;
	case Items::HoverFlashingRed:
		break;
	case Items::GunGrey:
		break;
	case Items::GunFlashingRed:
		break;
	case Items::HomingMissile:
		break;
	case Items::MultiwarheadMissile:
		break;
	case Items::ThunderBreak:
		break;
	case Items::HyperBeam:
		break;
	default:
		break;
	}
}

void CGameGlobal::beingAttackedByEnemy()
{
	//at here, I decrease -5 health player with all cases	
	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;

	switch (idPlayer)
	{
	case CLASS_SOPHIA:
		this->healthSophia -= BODY_DAMAGE_ENEMY;
		break;
	case CLASS_JASONSIDEVIEW:
		this->healthJasonSideView -= BODY_DAMAGE_ENEMY;
		break;
	default:
		this->healthJasonOverHead -= BODY_DAMAGE_ENEMY;
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
	if (this->healthJasonOverHead < 0) this->healthJasonOverHead = 0;
}

void CGameGlobal::beingAttackedByBullet()
{
	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;

	switch (idPlayer)
	{
	case CLASS_SOPHIA:
		this->healthSophia -= BODY_DAMAGE_BULLET;
		break;
	case CLASS_JASONSIDEVIEW:
		this->healthJasonSideView -= BODY_DAMAGE_BULLET;
		break;
	default:
		this->healthJasonOverHead -= BODY_DAMAGE_BULLET;
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
	if (this->healthJasonOverHead < 0) this->healthJasonOverHead = 0;
}

void CGameGlobal::beingAttackedByLava()
{

	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;

	if (idPlayer == CLASS_SOPHIA)
		this->healthSophia -= BODY_DAMAGE_LAVA;

	if (idPlayer == CLASS_JASONSIDEVIEW)
		this->healthJasonSideView -= BODY_DAMAGE_LAVA * 2;

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;

}

void CGameGlobal::beingAttackedBySpike()
{

	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;
	switch (idPlayer)
	{
	case CLASS_SOPHIA:
		this->healthSophia -= BODY_DAMAGE_SPIKE;
		break;
	case CLASS_JASONSIDEVIEW:
		this->healthJasonSideView -= BODY_DAMAGE_SPIKE*3;
		break;
	default:
		this->healthJasonOverHead -= BODY_DAMAGE_SPIKE;
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
	if (this->healthJasonOverHead < 0) this->healthJasonOverHead = 0;
}

void CGameGlobal::beingAttackedByDrop()
{
	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;
	if (idPlayer == CLASS_JASONSIDEVIEW)
	{
		this->healthJasonSideView -= BODY_DAMAGE_FALL;
	}
	
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
}

void CGameGlobal::beingAttackedByLowFall()
{
	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;
	if (idPlayer == CLASS_JASONSIDEVIEW)
	{
		this->healthJasonSideView -= BODY_DAMAGE_FALL/4;
	}

	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
}

void CGameGlobal::jasonJumpIntoSophia()
{
	healthJasonSideView = MAX_HEALTH_JASONSIDEVIEW;
}
void CGameGlobal::RenderHeath()
{
	int ID_Player = CGame::GetInstance()->GetCurrentPlayer()->classId;
	switch (ID_Player)
	{
		case CLASS_SOPHIA:
			SupportRenderHeath(healthSophia);
			break;
		case CLASS_JASONSIDEVIEW:
			SupportRenderHeath(healthJasonSideView);
			break;
		default:
			SupportRenderHeath(healthJasonOverHead);
			break;
	}
}

void CGameGlobal::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	//int R = atoi(tokens[2].c_str());
	//int G = atoi(tokens[3].c_str());
	//int B = atoi(tokens[4].c_str());

	int R = 133;
	int G = 123;
	int B = 212;

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void CGameGlobal::_ParseSection_SPRITES(string line)
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
void CGameGlobal::_ParseSection_ANIMATIONS(string line)
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
void CGameGlobal::_ParseSection_STATE_ANIMATION(string line)
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
void CGameGlobal::_ParseSection_OBJECT_ANIMATIONS(string line)
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

void CGameGlobal::SupportRenderHeath(int health)
{
	int flag_StateHealth = HEALTH0;
	if (health > 0 && health <= 10)
		flag_StateHealth = HEALTH1;
	if (health > 10 && health <= 20)
		flag_StateHealth = HEALTH2;
	if (health > 20 && health <= 30)
		flag_StateHealth = HEALTH3;
	if (health > 30 && health <= 40)
		flag_StateHealth = HEALTH4;
	if (health > 40 && health <= 50)
		flag_StateHealth = HEALTH5;
	if (health > 50 && health <= 60)
		flag_StateHealth = HEALTH6;
	if (health > 60 && health <= 70)
		flag_StateHealth = HEALTH7;
	if (health > 70)
		flag_StateHealth = HEALTH8;

	float X_cam, Y_cam;
	CGame::GetInstance()->GetCamPos(X_cam, Y_cam);
	HealthPow[flag_StateHealth]->Render(X_cam + X_HEALTH, Y_cam + Y_HEALTH);
}

void CGameGlobal::resetHealth()
{
	this->healthSophia = MAX_HEALTH_SOPHIA;
	this->healthJasonSideView = MAX_HEALTH_JASONSIDEVIEW;
	this->healthJasonOverHead = MAX_HEALTH_JASONOVERHEAD;
}

void CGameGlobal::saveGame()
{
	CGame * game = CGame::GetInstance();
	game->GetCurrentPlayer()->GetPosition(playerX, playerY);
}

void CGameGlobal::savePlayer(int kindPlayer)
{

	this->Saved = true;
	CGame* game = CGame::GetInstance();
	game->GetCurrentPlayer()->GetPosition(playerX, playerY);

	flagPlayer = kindPlayer;
	IDCurrentSection = game->GetCurrentPlayer()->currentSectionId;

	DebugOut(L"\n Save game %d, x : %f , y : %f ", kindPlayer, playerX, playerY);
}
void CGameGlobal::subLeft()
{
	if (left == -1) return;
	left--;
}

void CGameGlobal::resetGame()
{
	left = 3;
	resetHealth();
	this->Saved = false;
}

void CGameGlobal::saveSophia()
{
	CSophia * sophia = CSophia::GetInstance();
	sophia->GetPosition(this->sophiaX, this->sophiaY);

	this->IDSectionSophia = sophia->currentSectionId;
}