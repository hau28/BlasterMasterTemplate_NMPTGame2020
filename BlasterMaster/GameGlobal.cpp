#include "GameGlobal.h"
#include "Sophia.h"
#include "JasonSideview.h"
#include "JasonOverhead.h"
#include "Sound.h"
#include "GameObjectBehaviour.h"
#include "CComeBackAfterCrusherEvent.h"

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

#define GUN0 11111


#define X_HEALTH 20
#define Y_HEALTH 120
#define X_GUN 20
#define Y_GUN 80

//Weapon
#define ID_WEAPONMENU 11111
#define STATE_WEAPON 99999
#define ID_NUMBER 22222
#define ID_SELECTION_LEFT 33333
#define ID_SELECTION_RIGHT 44444
#define STATE_SELECTION_LEFT 11010
#define STATE_SELECTION_RIGHT	11020
#define ID_GUN 55555
#define ID_HEALTHOVERHEAD 66666
#define ID_EFFECT 77777
#define ID_FADE_IN 88888
#define ID_FADE_OUT 99999
#define ID_STATE_EFFECT 12000
#define ID_STATE_EFFECT_FADE_IN 13000

#define HEALTHOH0 9431

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

#define POS_X_SELECTED1	72
#define POS_X_SELECTED2	104
#define POS_X_SELECTED3	136
#define POS_Y_SELECTED	173

CGameGlobal* CGameGlobal::_instance = nullptr;

CGameGlobal::CGameGlobal() {
	//init healthSophia
	this->healthSophia = MAX_HEALTH_SOPHIA;
	this->healthJason = MAX_HEALTH_JASONSIDEVIEW;
	this->healthJason= MAX_HEALTH_JASONOVERHEAD;

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

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_SELECTION_LEFT);
	SelectedLeft = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_SELECTION_RIGHT);
	SelectedRight = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_GUN);
	Gun = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_HEALTHOVERHEAD);
	HealthOverhead = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_EFFECT);
	EffectFaded = objAnims->GenerateAnimationHanlders();

	objAnims = CObjectAnimationsLib::GetInstance()->Get(ID_FADE_IN);
	EffectFadedIn = objAnims->GenerateAnimationHanlders();

	//Init timer effect
	this->isEffectBoss = false;
	effectBossFlashingTimer = new CTimer(this, BOSS_EFFECT_DURATION, 1);
	effectBossFlashingTimer->Stop();
	
	this->isEffectBossFadeIn = false;
	effectBossFadeInTimer = new CTimer(this, BOSS_EFFECT_FADE_IN_DURATION, 1);
	effectBossFadeInTimer->Stop();

	this->isRenderBoss = false;
	this->isRenderBossFlashing = false;
	effectBossRenderFlashing = new CTimer(this, BOSS_EFFECT_RENDER_FLASHING, 1);
	effectBossRenderFlashing->Stop();

	times_render_boss = 0;
	effectBossRender = new CTimer(this, BOSS_EFFECT_RENDER, 1);
	effectBossRender->Stop();

	effectBossBossDead = new CTimer(this, BOSS_EFFECT_DEAD, 1);
	effectBossBossDead->Stop();
}

void CGameGlobal::Update(DWORD dt)
{
	SelectedLeft[STATE_SELECTION_LEFT]->Update();
	SelectedRight[STATE_SELECTION_RIGHT]->Update();
}

void CGameGlobal::UpdateEffect(DWORD dt)
{
	effectBossFadeInTimer->Update(dt);
	effectBossFlashingTimer->Update(dt);
	effectBossRender->Update(dt);
	effectBossRenderFlashing->Update(dt);
	effectBossBossDead->Update(dt);

	if (isEffectFaded)
	{
		EffectFaded[ID_STATE_EFFECT]->Update();
		if (EffectFaded[ID_STATE_EFFECT]->currentFrameIndex == EffectFaded[ID_STATE_EFFECT]->animation->GetNumberOfFrames() - 1)
		{
			isEffectFaded = false;
			EffectFaded[ID_STATE_EFFECT]->currentFrameIndex = 0;
		}
	}
	if (isEffectBossFadeIn)
	{
		EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->Update();
		if (EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->currentFrameIndex == EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->animation->GetNumberOfFrames() - 1)
		{
			isEffectBossFadeIn = false;
			EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->currentFrameIndex = 0;
		}
	 }
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
		this->healthJason -= BODY_DAMAGE_ENEMY;
		break;
	case CLASS_JASONOVERHEAD:
		this->healthJason -= BODY_DAMAGE_ENEMY;
		AddToGunLevel(-1);
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJason < 0) this->healthJason = 0;
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
		this->healthJason-= BODY_DAMAGE_BULLET;
		break;
	default:
		this->healthJason -= BODY_DAMAGE_BULLET;
		AddToGunLevel(-1);
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJason < 0) this->healthJason = 0;
}

void CGameGlobal::beingAttackedByLava()
{

	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;

	if (idPlayer == CLASS_SOPHIA)
		this->healthSophia -= BODY_DAMAGE_LAVA;

	if (idPlayer == CLASS_JASONSIDEVIEW)
		this->healthJason -= BODY_DAMAGE_LAVA * 2;

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJason < 0) this->healthJason = 0;

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
		this->healthJason -= BODY_DAMAGE_SPIKE*3;
		break;
	default:
		this->healthJason -= BODY_DAMAGE_SPIKE;
		AddToGunLevel(-1);
		break;
	}

	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJason < 0) this->healthJason = 0;
}

void CGameGlobal::beingAttackedByDrop()
{
	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;
	if (idPlayer == CLASS_JASONSIDEVIEW)
	{
		this->healthJason -= BODY_DAMAGE_FALL;
	}
	
	if (this->healthJason < 0) this->healthJason = 0;
}
void CGameGlobal::beingAttackedByLowFall()
{
	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;
	if (idPlayer == CLASS_JASONSIDEVIEW || idPlayer == CLASS_JASONOVERHEAD)
	{
		this->healthJason -= BODY_DAMAGE_FALL/4;
	}

	if (this->healthJason < 0) this->healthJason = 0;
}
void CGameGlobal::jasonJumpIntoSophia()
{
	healthJason = MAX_HEALTH_JASONSIDEVIEW;
}

void CGameGlobal::AddToSophiaHealth(int amount)
{
	healthSophia += amount;
	healthSophia = max(healthSophia, 0);
	healthSophia = min(healthSophia, MAX_HEALTH_SOPHIA);
}
void CGameGlobal::AddToJasonHealth(int amount)
{
	healthJason += amount;
	healthJason = max(healthJason, 0);
	healthJason = min(healthJason, MAX_HEALTH_JASONSIDEVIEW);
}

void CGameGlobal::AddToGunLevel(int amount)
{
	levelGunPower += amount;
	levelGunPower = max(levelGunPower, 0);
	levelGunPower = min(levelGunPower, MAX_GUN_LEVEL);
}

void CGameGlobal::RenderHealth()
{
	int ID_Player = CGame::GetInstance()->GetCurrentPlayer()->classId;
	switch (ID_Player)
	{
		case CLASS_SOPHIA:
			SupportRenderHeath(healthSophia);
			break;
		case CLASS_JASONSIDEVIEW:
			SupportRenderHeath(healthJason);
			break;
		default:
			SupportRenderHeath(healthJason);
			break;
	}
}
void CGameGlobal::RenderHealthGun()
{
	float X_cam, Y_cam;
	CGame::GetInstance()->GetCamPos(X_cam, Y_cam);
	int ID_STATE_GUN = GUN0 + this->levelGunPower;
	Gun[ID_STATE_GUN]->Render(X_GUN + X_cam, Y_GUN + Y_cam);

	//Render health 
	int health = this->healthJason;
	int flag_StateHealth = 0;
	if (health > 0 && health <= 10)
		flag_StateHealth = 1;
	if (health > 10 && health <= 20)
		flag_StateHealth = 2;
	if (health > 20 && health <= 30)
		flag_StateHealth = 3;
	if (health > 30 && health <= 40)
		flag_StateHealth = 4;
	if (health > 40 && health <= 50)
		flag_StateHealth = 5;
	if (health > 50 && health <= 60)
		flag_StateHealth = 6;
	if (health > 60 && health <= 70)
		flag_StateHealth = 7;
	if (health > 70)
		flag_StateHealth = 8;
	CGame::GetInstance()->GetCamPos(X_cam, Y_cam);
	HealthOverhead[flag_StateHealth + HEALTHOH0]->Render(X_cam + X_HEALTH, Y_cam + Y_HEALTH + 30);
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

	//Draw animation selection item
	float X_Selected, Y_Selected;
	const int OFFSETBETWEEN = 32;
	Y_Selected = POS_Y_SELECTED + Y_cam;
	switch (this->idSelectedItem)
	{
	case 1:
		X_Selected = POS_X_SELECTED1 + X_cam;
		break;
	case 2:
		X_Selected = POS_X_SELECTED2 + X_cam;
		break;
	case 3:
		X_Selected = POS_X_SELECTED3 + X_cam;
		break;
	default:
		X_Selected = POS_X_SELECTED1 + X_cam;
		break;
	}
	SelectedLeft[STATE_SELECTION_LEFT]->Render(X_Selected, Y_Selected);
	SelectedRight[STATE_SELECTION_RIGHT]->Render(X_Selected + OFFSETBETWEEN, Y_Selected);
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
	this->healthJason = MAX_HEALTH_JASONSIDEVIEW;

	this->stateBossBlackBackground = false;
}

void CGameGlobal::saveGame()
{
	CGame * game = CGame::GetInstance();
	game->GetCurrentPlayer()->GetPosition(playerX, playerY);
}

void CGameGlobal::savePlayer(int kindPlayer, float offx, float offy)
{
	this->Saved = true;
	CGame* game = CGame::GetInstance();
	game->GetCurrentPlayer()->GetPosition(playerX, playerY);

	playerX += offx;
	playerY += offy;

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

	//hoi ky nhi?
	this->stateBossBlackBackground = false;
	this->isDeadBoss = false;
	this->HasCrusherBeam = false;
	this->isWinGame = false;
}

void CGameGlobal::saveSophia(float offx, float offy)
{
	CSophia * sophia = CSophia::GetInstance();
	sophia->GetPosition(this->sophiaX, this->sophiaY);

	this->IDSectionSophia = sophia->currentSectionId;
	this->sophiaX += offx;
	this->sophiaY += offy;
}

void CGameGlobal::saveJason(float offx, float offy)
{
	isOverheadtoSideView = true;
	CJasonSideview* jason = CJasonSideview::GetInstance();
	jason->GetPosition(this->jasonX, this->jasonY);

	this->IDSectionJason = jason->currentSectionId;
	this->jasonX += offx;
	this->jasonY += offy;
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
		AddToHomingMissile(amount);
		break;

	case TypeWeapons::ThunderBreak:
		AddToThunderBreak(amount);
		break;

	case TypeWeapons::MultiwarheadMissile:
		AddToMultiwarheadMissile(amount);
		break;
	}
}

void CGameGlobal::AddToHomingMissile(int amount)
{
	ammunitions_HomingMissile += amount;
	ammunitions_HomingMissile = min(ammunitions_HomingMissile, MAX_AMMUNITIONS);
	ammunitions_HomingMissile = max(ammunitions_HomingMissile, 0);
}

void CGameGlobal::AddToMultiwarheadMissile(int amount)
{
	ammunitions_MultiwarheadMissile += amount;
	ammunitions_MultiwarheadMissile = min(ammunitions_MultiwarheadMissile, MAX_AMMUNITIONS);
	ammunitions_MultiwarheadMissile = max(ammunitions_MultiwarheadMissile, 0);
}

void CGameGlobal::AddToThunderBreak(int amount)
{
	ammunitions_ThunderBreak += amount;
	ammunitions_ThunderBreak = min(ammunitions_ThunderBreak, MAX_AMMUNITIONS);
	ammunitions_ThunderBreak = max(ammunitions_ThunderBreak, 0);
}

void CGameGlobal::OpenMenuWeapon()
{
	//Update weapon selected
	switch (this->selectedWeapon)
	{
	case TypeWeapons::HomingMissile:
		idSelectedItem = 1;
		break;
	case TypeWeapons::ThunderBreak:
		idSelectedItem = 2;
		break;
	case TypeWeapons::MultiwarheadMissile:
		idSelectedItem = 3;
		break;
	default:
		break;
	}

	this->isWeaponMenuActive = true;
}

void CGameGlobal::CloseMenuWeapon()
{
	this->isWeaponMenuActive = false;

	//Update weapon selected
	switch (idSelectedItem)
	{
		case 1:
			this->selectedWeapon = TypeWeapons::HomingMissile;
			break;
		case 2:
			this->selectedWeapon = TypeWeapons::ThunderBreak;
			break;
		case 3:
			this->selectedWeapon = TypeWeapons::MultiwarheadMissile;
			break;
		default:
			this->selectedWeapon = TypeWeapons::HomingMissile;
		break;
	}
}

//Control key Weapon
void CGameGlobal::NextSelectedItem()
{
	if (this->idSelectedItem < 3)
		this->idSelectedItem++;
	else
		this->idSelectedItem = 1;
}

void CGameGlobal::BackSelectedItem()
{
	if (this->idSelectedItem > 1)
		this->idSelectedItem--;
	else
		this->idSelectedItem = 3;
}

//Effect Faded boss
void CGameGlobal::RenderEffect()
{
	float camx, camy;
	CGame::GetInstance()->GetCamPos(camx, camy);
	if (this->isEffectFaded)
	{
		EffectFaded[ID_STATE_EFFECT]->Render(camx, camy);
	}
	if (this->isEffectBossFadeIn)
	{
		EffectFadedIn[ID_STATE_EFFECT_FADE_IN]->Render(camx, camy);
	}
}

void CGameGlobal::initEffectFaded()
{
	this->isEffectFaded = true; 
}

void CGameGlobal::openEffectFlashingBoss()
{
	Sound::getInstance()->stop(AREA2);
	Sound::getInstance()->play(BOSS_ENTER, false, 1);
	this->isEffectBoss = true;
	this->stateBossBlackBackground = false;
	this->isEffectBossFadeIn = false;
	this->isRenderBoss = false;
	this->isRenderBossFlashing = false;
	this->times_render_boss = 0;
	this->effectBossFlashingTimer->Start();
}

void CGameGlobal::HandleTimerTick(LPTIMER sender)
{
	if (sender == effectBossFlashingTimer)
	{
		this->isEffectBoss = false;
		this->isEffectBossFadeIn = true;
		effectBossFadeInTimer->Start();
	}
	if (sender == effectBossFadeInTimer)
	{
		this->isEffectBossFadeIn = false;
		stateBossBlackBackground = true;
		this->effectBossRender->Start();
	}
	if (sender == effectBossRender) 
	{
		Sound::getInstance()->stop(BOSS_ENTER);
		Sound::getInstance()->play(BOSS, false, 1);
		this->isRenderBoss = true;
		this->effectBossRender->Stop();
		this->effectBossRenderFlashing->Start();
	}
	if (sender == effectBossRenderFlashing)
	{
		times_render_boss++;
		if (times_render_boss <= 7)
		{
			//Flashing boss
			this->effectBossRenderFlashing->Start();
		}
		else
		{
			times_render_boss = 0;
			this->isRenderBossFlashing = false;
			this->effectBossRenderFlashing->Stop();
		}
	}
	if (sender == effectBossBossDead)
	{
		this->stateBossBlackBackground = false;
		//create crushed beam
		LPITEM gunItem = new CItem(CLASS_ITEM_CRUSHERBEAM, 100, 100, ID_SECTION_BOSSOVERHEAD, false);
		CGameObjectBehaviour::CreateObject(gunItem);
	}
}

void CGameGlobal::openEffectBossDead()
{
	this->effectBossBossDead->Start();
}

void CGameGlobal::comeBackAfterGetCrusherBeam()
{
	CComeBackAfterCrusherEvent* newEvent = new CComeBackAfterCrusherEvent(sectionCrusherBeamComeback);
	CGame::GetInstance()->AddGameEvent(newEvent);
}