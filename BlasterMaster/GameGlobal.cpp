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


//Weapon
#define ID_WEAPONMENU 11111
#define STATE_WEAPON 99999
#define ID_NUMBER 22222

#define NUMBER_0	11000
#define NUMBER_1	11001
#define NUMBER_2	11002
#define NUMBER_3	11003
#define NUMBER_4	11004
#define NUMBER_5	11005
#define NUMBER_6	11006
#define NUMBER_7	11007
#define NUMBER_8	11008
#define NUMBER_9	11009

#define POS_Y_NUMBER 190
#define POS_X_NUMBER1	86
#define POS_X_NUMBER2	96
#define POS_X_NUMBER3	118
#define POS_X_NUMBER4	128
#define POS_X_NUMBER5	150
#define POS_X_NUMBER6	160

CGameGlobal* CGameGlobal::_instance = nullptr;

CGameGlobal::CGameGlobal() {
	//init healthSophia
	this->healthSophia = MAX_HEALTH_SOPHIA;
	this->healthJasonSideView = MAX_HEALTH_JASONSIDEVIEW;
	this->healthJasonOverHead = MAX_HEALTH_JASONOVERHEAD;

	//init level Gun
	this->levelGunPower = 0;
	
	ifstream f;
	f.open(L"HEALTHANDGUNANDWEAPON.txt");

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

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_WEAPONMENU);
	WeaponMenu = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_NUMBER);
	Numbers = objAnims->GenerateAnimationHanlders();
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

void CGameGlobal::beingAttackedByLava()
{

	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;

	switch (idPlayer)
	{
	case CLASS_SOPHIA:
		this->healthSophia -= BODY_DAMAGE_LAVA;
		break;
	case CLASS_JASONSIDEVIEW:
		this->healthJasonSideView -= BODY_DAMAGE_LAVA*2;
		break;
	default:
		this->healthJasonOverHead -= BODY_DAMAGE_LAVA;
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
	if (this->healthJasonOverHead < 0) this->healthJasonOverHead = 0;
}

void CGameGlobal::beingAttackedBySpike()
{

	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;
	switch (idPlayer)
	{
	case CLASS_SOPHIA:
		this->healthSophia -= BODY_DAMAGE_SPIKE/3;
		break;
	case CLASS_JASONSIDEVIEW:
		this->healthJasonSideView -= BODY_DAMAGE_SPIKE;
		break;
	default:
		this->healthJasonOverHead -= BODY_DAMAGE_SPIKE;
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
	if (this->healthJasonOverHead < 0) this->healthJasonOverHead = 0;
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

void CGameGlobal::RenderWeapon()
{
	if (!isMenuWeaponOpen())
		return;
	float X_cam, Y_cam;
	CGame::GetInstance()->GetCamPos(X_cam, Y_cam);
	WeaponMenu[STATE_WEAPON]->Render(X_cam, Y_cam);

	//analys number 
	int number1, number2, number3, number4, number5, number6;
	SupportAnalysNumber(this->ammunitions_HomingMissile, number1, number2);
	SupportAnalysNumber(this->ammunitions_ThunderBreak, number3, number4);
	SupportAnalysNumber(this->ammunitions_MultiwarheadMissile, number5, number6);

	Numbers[SupportGetIDNumber(number1)]->Render(POS_X_NUMBER1 + X_cam, POS_Y_NUMBER + Y_cam);
	Numbers[SupportGetIDNumber(number2)]->Render(POS_X_NUMBER2 + X_cam, POS_Y_NUMBER + Y_cam);
	Numbers[SupportGetIDNumber(number3)]->Render(POS_X_NUMBER3 + X_cam, POS_Y_NUMBER + Y_cam);
	Numbers[SupportGetIDNumber(number4)]->Render(POS_X_NUMBER4 + X_cam, POS_Y_NUMBER + Y_cam);
	Numbers[SupportGetIDNumber(number5)]->Render(POS_X_NUMBER5 + X_cam, POS_Y_NUMBER + Y_cam);
	Numbers[SupportGetIDNumber(number6)]->Render(POS_X_NUMBER6 + X_cam, POS_Y_NUMBER + Y_cam);
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

int CGameGlobal::SupportGetIDNumber(int number)
{
	switch (number)
	{
	case 0:
		return NUMBER_0;
	case 1:
		return NUMBER_1;
	case 2:
		return NUMBER_2;
	case 3:
		return NUMBER_3;
	case 4:
		return NUMBER_4;
	case 5:
		return NUMBER_5;
	case 6:
		return NUMBER_6;
	case 7:
		return NUMBER_7;
	case 8:
		return NUMBER_8;
	case 9:
		return NUMBER_9;
	default:
		return NUMBER_0;
		break;
	}
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
void CGameGlobal::SupportAnalysNumber(int number, int& first, int& second)
{
	if (number >= 100)
	{
		first = 9;
		second = 9;
		return;
	}

	if (number <= 0)
	{
		first = 0;
		second = 0;
	}

	first = number / 10;
	second = number % 10;
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
bool CGameGlobal::CheckSophiaCanUseWeapon()
{
	switch (selectedWeapon)
	{
	case TypeWeapons::HomingMissile:
		return ammunitions_HomingMissile > 0;
	case TypeWeapons::ThunderBreak:
		return ammunitions_ThunderBreak > 0;
	case TypeWeapons::MultiwarheadMissile:
		return ammunitions_MultiwarheadMissile > 0;
	default:
		return false;
	}
}

void CGameGlobal::AddToSelectedWeapon(int amount)
{
	// CuteTN confess: it's too WET here...
	switch (selectedWeapon)
	{
	case TypeWeapons::HomingMissile:
		ammunitions_HomingMissile += amount;
		ammunitions_HomingMissile = min(ammunitions_HomingMissile, MAX_AMMUNITIONS);
		ammunitions_HomingMissile = max(ammunitions_HomingMissile, 0);

	case TypeWeapons::ThunderBreak:
		ammunitions_ThunderBreak += amount;
		ammunitions_ThunderBreak = min(ammunitions_ThunderBreak, MAX_AMMUNITIONS);
		ammunitions_ThunderBreak = max(ammunitions_ThunderBreak, 0);

	case TypeWeapons::MultiwarheadMissile:
		ammunitions_MultiwarheadMissile += amount;
		ammunitions_MultiwarheadMissile = min(ammunitions_MultiwarheadMissile, MAX_AMMUNITIONS);
		ammunitions_MultiwarheadMissile = max(ammunitions_MultiwarheadMissile, 0);
	}
}

void CGameGlobal::OpenMenuWeapon()
{
	this->isWeaponMenuActive = true;
}
void CGameGlobal::CloseMenuWeapon()
{
	this->isWeaponMenuActive = false;
}