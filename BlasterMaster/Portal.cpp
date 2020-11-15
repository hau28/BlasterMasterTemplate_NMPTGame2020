#include "Portal.h"

CPortal::CPortal(int classId, int x, int y, int width, int height, int sectionId, int associatedPortalId) : CTileArea(classId, x, y, width, height, sectionId)
{
	this->associatedPortalId = associatedPortalId;
}
