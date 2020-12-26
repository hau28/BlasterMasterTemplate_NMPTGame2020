#pragma once
#include "Utils.h"
#include <map>

#include "Dome.h"
#include "Floater.h"
#include "TileArea.h"
#include "Sophia.h"
#include "Panda.h"
#include "Ship.h"
#include "Portal.h"
#include "JasonSideview.h"
#include "JasonOverhead.h"
#include "Mine.h"
#include "Skull.h"
#include "Jumper.h"
#include "Orb.h"
#include "Insect.h"

/// <summary>
/// The Abstract factory for creating game objects
/// </summary>
class CGameObjectFactory
{
	static void GetAnimatableObjectProps(map<string, string> properties, int& x, int& y, int& animsId, int& sectionId);
	static void GetTileAreaObjectProps(map<string, string> properties, int& x, int& y, int& width, int& height, int&sectionId);
	static void GetPortalProps(map<string, string> properties, int& x, int& y, int& width, int& height, int&sectionId, int&associatedPortalId);
	static int GetSideviewEquivalentClassId(int classId);

public:
	/// <summary>
	/// Create a game object
	/// </summary>
	/// <param name="scene">Inject the scene of the current game object to add it to data</param>
	/// <param name="classId">Decide which class of object to create</param>
	/// <param name="properties">the list of properties info, map from a property name to its corresponding value</param>
	/// <returns></returns>
	static LPGAMEOBJECT Create(int classId, map<string, string> properties, int& sectionId);
};

