#include "GameGlobal.h"

CGameGlobal* CGameGlobal::_instance = nullptr;

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
