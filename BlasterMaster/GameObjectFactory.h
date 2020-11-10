#pragma once
#include "Utils.h"
#include <map>

#include "Dome.h"
#include "Floater.h"
#include "TileArea.h"
#include "Sophia.h"
#include "Worm.h"

/// <summary>
/// The Abstract factory for creating game objects
/// </summary>
class CGameObjectFactory
{
	static void GetAnimatableObjectProps(map<string, string> properties, int& x, int& y, int& animsId);
	static void GetTileAreaObjectProps(map<string, string> properties, int& x, int& y, int& width, int& height);

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

