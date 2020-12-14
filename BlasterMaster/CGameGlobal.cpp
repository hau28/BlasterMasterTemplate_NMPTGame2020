#include "CGameGlobal.h"

CGameGlobal::CGameGlobal() {
	//init healthSophia
	this->healthSophia = MAX_HEALTH_SOPHIA;
	this->healthJasonSideView = MAX_HEALTH_JASONSIDEVIEW;
	this->healthJasonOverHead = MAX_HEALTH_JASONOVERHEAD;

	//init level Gun
	this->levelGunPower = 0;
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
void CGameGlobal::beingAttackedByEnemy(CEnemy* enemy)
{
	//at here, I decrease -5 health player with all cases	
	int idPlayer = CGame::GetInstance()->GetCurrentPlayer()->classId;
	switch (idPlayer)
	{
	case CLASS_SOPHIA:
		this->healthSophia -= 5;
		break;
	case CLASS_JASONSIDEVIEW:
		this->healthJasonSideView -= 5;
		break;
	default:
		this->healthJasonOverHead -= 5;
		break;
	}
	
	if (this->healthSophia < 0) this->healthSophia = 0;
	if (this->healthJasonSideView < 0) this->healthJasonSideView = 0;
	if (this->healthJasonOverHead < 0) this->healthJasonOverHead = 0;
}