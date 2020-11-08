#include "GameObjectFactory.h"

LPGAMEOBJECT CGameObjectFactory::Create(int classId, map<string, string> properties, int& sectionId)
{
	LPGAMEOBJECT result = nullptr;
	sectionId = -1;

	// CuteTN Todo...
	// not yet have any game object, so leave this here until we can finally add it...
	
	int x, y, width, height, animsId;

	switch (classId)
	{
	case CLASS_DOME:
		GetAnimatableObjectProps(properties, x, y, animsId);
		result = new CDome(classId, x, y, animsId);
		break;
	case CLASS_FLOATER1:
		GetAnimatableObjectProps(properties, x, y, animsId);
		result = new CFloater(classId, x, y, animsId);
		break;
	case CLASS_TILE_BLOCKABLE:
		GetTileAreaObjectProps(properties, x, y, width, height);
		result = new CTileArea(classId, x, y, width, height);
		break;
	default:
		return nullptr;
		break;
	}

	//every game object should have a section
	sectionId = atoi(properties["Section"].c_str());

	return result;
}

void CGameObjectFactory::GetAnimatableObjectProps(map<string, string> properties, int&x, int&y, int&animsId)
{
	x = atoi(properties["X"].c_str());
	y = atoi(properties["Y"].c_str());
	animsId = atoi(properties["Animations"].c_str());
}

void CGameObjectFactory::GetTileAreaObjectProps(map<string, string> properties, int& x, int& y, int& width, int& height)
{
	x = atoi(properties["X"].c_str());
	y = atoi(properties["Y"].c_str());
	width = atoi(properties["Width"].c_str());
	height = atoi(properties["Height"].c_str());
}
