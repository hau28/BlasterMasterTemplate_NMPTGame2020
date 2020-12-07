#include "GameObjectFactory.h"

LPGAMEOBJECT CGameObjectFactory::Create(int classId, map<string, string> properties, int& sectionId)
{
	LPGAMEOBJECT result = nullptr;
	sectionId = -1;

	int x, y, width, height, animsId, portalId, initLeft, rotation;

	switch (classId)
	{
	case CLASS_DOME:
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = new CDome(classId, x, y, sectionId, animsId);
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

	case CLASS_SOPHIA:
		// CuteTN Note: maybe refactor the order of parameters...
		GetAnimatableObjectProps(properties, x, y, animsId, sectionId);
		result = CSophia::InitInstance(classId, x, y, sectionId);
		break;

	case CLASS_TILE_BLOCKABLE:
		GetTileAreaObjectProps(properties, x, y, width, height, sectionId);
		result = new CTileArea(classId, x, y, width, height, sectionId);
		break;

	case CLASS_TILE_PORTAL:
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

	default:
		return nullptr;
		break;
	}

	//every game object should have a section
	sectionId = atoi(properties["Section"].c_str());

	return result;
}

void CGameObjectFactory::GetAnimatableObjectProps(map<string, string> properties, int&x, int&y, int&animsId, int&sectionId)
{
	x = atoi(properties["X"].c_str());
	y = atoi(properties["Y"].c_str());
	animsId = atoi(properties["Animations"].c_str());
	sectionId = atoi(properties["Section"].c_str());
}

void CGameObjectFactory::GetTileAreaObjectProps(map<string, string> properties, int& x, int& y, int& width, int& height, int&sectionId)
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
