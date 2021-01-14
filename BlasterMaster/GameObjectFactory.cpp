#include "GameObjectFactory.h"

LPGAMEOBJECT CGameObjectFactory::Create(int classId, map<string, string> properties, int& sectionId)
{
	LPGAMEOBJECT result = nullptr;
	sectionId = -1;

	int x, y, width, height, animsId, portalId, initLeft, rotation, behaviorId, isClockwise, isFlashy;

	// CuteTN Note: dirty dirty very dirty code here
	// convert classId of scene Overhead into the equivalent ones in Sideview
	classId = GetSideviewEquivalentClassId(classId);
	

	switch (classId)
	{
	case CLASS_DOME:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		rotation = atoi(properties["Rotation"].c_str());
		isClockwise = atoi(properties["IsClockwise"].c_str());
		result = new CDome(classId, x, y, sectionId, rotation, isClockwise, animsId);
		break;

	case CLASS_FLOATER:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = new CFloater(classId, x, y, sectionId, animsId);
		break;

	case CLASS_PANDA:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = new CPanda(classId, x, y, sectionId, animsId);
		break;

	case CLASS_SHIP:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		initLeft = atoi(properties["InitLeft"].c_str());
		result = new CShip(classId, x, y, sectionId, initLeft, animsId);
		break;

	case CLASS_JUMPER:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		initLeft = atoi(properties["InitLeft"].c_str());
		result = new CJumper(classId, x, y, sectionId, initLeft, animsId);
		break;

	case CLASS_ORB:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		initLeft = atoi(properties["InitLeft"].c_str());
		behaviorId = atoi(properties["Aggressive"].c_str());
		result = new COrb(classId, x, y, sectionId, initLeft, animsId, behaviorId);
		break;

	case CLASS_SOPHIA:
		// CuteTN Note: maybe refactor the order of parameters...
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = CSophia::InitInstance(classId, x, y, sectionId);
		break;

	case CLASS_TILE_BLOCKABLE:
	case CLASS_TILE_LADDER:
	case CLASS_TILE_LAVA:
	case CLASS_TILE_SPIKE:
		GetTileAreaObjectProps(properties, x, y, width, height, sectionId);
		result = new CTileArea(classId, x, y, width, height, sectionId);
		break;

	case CLASS_TILE_PORTAL:
	case CLASS_TILE_SCENEPORTAL:
		GetPortalProps(properties, x, y, width, height, sectionId, portalId);
		result = new CPortal(classId, x, y, width, height, sectionId, portalId);
		break;

	case CLASS_MINE:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = new CMine(classId, x, y, sectionId, animsId);
		break;

	case CLASS_SKULL:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = new CSkull(classId, x, y, sectionId, animsId);
		break;

	case CLASS_INSECT:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = new CInsect(classId, x, y, sectionId, animsId);
		break;

	case CLASS_BREAKABLE_BLUE_BG:
	case CLASS_BREAKABLE_GREEN_BG:
	case CLASS_BREAKABLE_OVERHEAD_BG:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = new CBreakableBlock(classId, x, y, sectionId, animsId);
		break;

	case CLASS_ITEM_GUN:
	case CLASS_ITEM_POWER:
	case CLASS_ITEM_HOVER:
	case CLASS_ITEM_MULTIWARHEADMISSILE:
	case CLASS_ITEM_HOMINGMISSILE:
	case CLASS_ITEM_THUNDERBREAK:
		GetItemProps(properties, x, y, animsId, sectionId, isFlashy);
		result = new CItem(classId, x, y, sectionId, isFlashy);
		break;

	default:
		return nullptr;
		break;
	}

	//every game object should have a section
	sectionId = atoi(properties["Section"].c_str());

	return result;
}

void CGameObjectFactory::GetAnimatableObjectProps(map<string, string> properties, int& x, int& y, int& animsId, int& sectionId)
{
	x = atoi(properties["X"].c_str());
	y = atoi(properties["Y"].c_str());
	animsId = atoi(properties["Animations"].c_str());
	sectionId = atoi(properties["Section"].c_str());
}

void CGameObjectFactory::GetTileAreaObjectProps(map<string, string> properties, int& x, int& y, int& width, int& height, int& sectionId)
{
	x = atoi(properties["X"].c_str());
	y = atoi(properties["Y"].c_str());
	width = atoi(properties["Width"].c_str());
	height = atoi(properties["Height"].c_str());
	sectionId = atoi(properties["Section"].c_str());
}

void CGameObjectFactory::GetPortalProps(map<string, string> properties, int& x, int& y, int& width, int& height, int& sectionId, int& associatedPortalId)
{
	GetTileAreaObjectProps(properties, x, y, width, height, sectionId);
	associatedPortalId = atoi(properties["Portal"].c_str());
}

void CGameObjectFactory::GetItemProps(map<string, string> properties, int& x, int& y, int& animsId, int& sectionId, int& isFlashy)
{
	GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
	isFlashy = atoi(properties["IsFlashy"].c_str());
}

int CGameObjectFactory::GetSideviewEquivalentClassId(int classId)
{
	switch (classId)
	{
	case CLASS_TILE_BLOCKABLE_O:
		return CLASS_TILE_BLOCKABLE;
	case CLASS_TILE_PORTAL_O:
		return CLASS_TILE_PORTAL;
	case CLASS_TILE_SCENEPORTAL_O:
		return CLASS_TILE_SCENEPORTAL;
	case CLASS_TILE_SPIKE_O:
		return CLASS_TILE_SPIKE;

	case CLASS_ITEM_POWER_O:
		return CLASS_ITEM_POWER;
	case CLASS_ITEM_HOVER_O:
		return CLASS_ITEM_HOVER;
	case CLASS_ITEM_GUN_O:
		return CLASS_ITEM_GUN;
	case CLASS_ITEM_HOMINGMISSILE_O:
		return CLASS_ITEM_HOMINGMISSILE;
	case CLASS_ITEM_MULTIWARHEADMISSILE_O:
		return CLASS_ITEM_MULTIWARHEADMISSILE;
	case CLASS_ITEM_THUNDERBREAK_O:
		return CLASS_ITEM_THUNDERBREAK;

	default:
		return classId;
	}
}

