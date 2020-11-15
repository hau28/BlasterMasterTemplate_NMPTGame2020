#pragma once
#include "TileArea.h"

class CPortal :
	public CTileArea
{
public:
    int associatedPortalId = -1;

    CPortal() {}
    CPortal(int classId, int x, int y, int width, int height, int sectionId, int associatedPortalId);
};

typedef CPortal* LPPORTAL;
