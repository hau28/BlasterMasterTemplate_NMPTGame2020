#include "PortalLib.h"

CPortalLib* CPortalLib::__instance = nullptr;

void CPortalLib::Add(int portalId, LPPORTAL portal)
{
	portals[portalId] = portal;
}

LPPORTAL CPortalLib::Get(int portalId)
{
	return portals[portalId];
}

void CPortalLib::Clear()
{
	portals.clear();
}

CPortalLib* CPortalLib::GetInstance()
{
	if (__instance == nullptr)
		__instance = new CPortalLib();

	return __instance;
}


