#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Game.h"
#include <map>
#include "Animations.h"

#include "Dome.h"
#include "Floater.h"
#include "CTileArea.h"

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
	static LPGAMEOBJECT Create(LPSCENE scene, int classId, map<string, string> properties);
};

