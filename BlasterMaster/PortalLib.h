#pragma once
#include "Portal.h"

class CPortalLib
{
private:
	CPortalLib() {};

	static CPortalLib* __instance;

	/// <summary>
	/// Map from [PortalId] to [LPPortal]
	/// </summary>
	unordered_map<int, LPPORTAL> portals;

public:
	void Add(int portalId, LPPORTAL portal);
	LPPORTAL Get(int portalId);
	void Clear();

	static CPortalLib* GetInstance();
}; 

