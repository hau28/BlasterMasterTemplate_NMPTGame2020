#include "Portal.h"

CPortal::CPortal(int classId, int x, int y, int width, int height, int sectionId, int associatedPortalId) : CTileArea(classId, x, y, width, height, sectionId)
{
	// CuteTN: dirty, but I'm out of time baby
	if (classId == CLASS_TILE_PORTAL)
		this->associatedPortalId = associatedPortalId;
	else if (classId == CLASS_TILE_SCENEPORTAL)
		this->port = associatedPortalId;
}
