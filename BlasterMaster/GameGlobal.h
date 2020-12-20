#pragma once

#include "Enemy.h"

#define MAX_HEALTH_SOPHIA 80
#define MAX_HEALTH_JASONSIDEVIEW 80
#define MAX_HEALTH_JASONOVERHEAD 80
#define BODY_DAMAGE_ENEMY 5
#define BODY_DAMAGE_LAVA 10

enum class Items {
	PowerGrey, // add 1 bar
	PowerFlashingRed, // add 4 bars
	HoverGrey, 
	HoverFlashingRed,
	GunGrey,
	GunFlashingRed,
	HomingMissile, // item weapon
	MultiwarheadMissile,// item weapon
	ThunderBreak,// item weapon
	HyperBeam, //item to destroy the guardian that stands between you and Area 2.
};

class CGameGlobal
{
private:
	CGameGlobal();
	static CGameGlobal * _instance;

	//AnimationHandler Render 
	CObjectAnimationHanlders HealthPow;
	CObjectAnimationHanlders Hov;
	CObjectAnimationHanlders Gun;

	//health player 
	int healthSophia;
	int healthJasonSideView;
	int healthJasonOverHead;

	//Gun Power
	int levelGunPower; // [0..8]

	//Weapons
	int ammunitions_HomingMissile = 0;
	int ammunitions_MultiwarheadMissile = 0;
	int ammunitions_ThunderBreak = 0;

	//Input for Render Pow & Gun
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_STATE_ANIMATION(string line);
	void _ParseSection_OBJECT_ANIMATIONS(string line);

	//Support funtion, U shouldn't care it 
	void SupportRenderHeath(int health);

public:
	//get value methods
	static CGameGlobal* GetInstance();
	int get_healthSophia()		  { return this->healthSophia;		  }
	int get_healthJasonSideView() { return this->healthJasonSideView; }
	int get_healthJasonOverHead() { return this->healthJasonOverHead; }

	//event method
	void getItem(Items item);
	void beingAttackedByEnemy();
	void beingAttackedByLava();
	void jasonJumpIntoSophia();
	//Render
	void RenderHeath();
};


//INFO LEVEL GUN POWER
//1	Jason fires shots that stop halfway across the screen.
//2	Jason can fire two shots that travel all the way across the screen.
//3	Jason can fire three shots that travel all the way across the screen.
//4	Jason can fire three shots that travel all the way across the screen.
//5	Some bullets that Jason fires will fly off in slowly expanding circles to either side.
//6	Some bullets that Jason fires will fly off in quickly expanding circles to either side.
//7	Jason can fire up to five bullets that travel in slowly expanding waves
//8	Jason can fire a stream of bullets that can pass through solid objects, and travel in quickly expanding waves.

//Also note that Jason can recover all of his power by returning to SOPHIA