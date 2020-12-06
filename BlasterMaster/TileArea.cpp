#include "TileArea.h"

void CTileArea::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void CTileArea::GetSize(int& width, int& height)
{
	width = this->width;
	height = this->height;
}

CTileArea::CTileArea(int classId, int x, int y, int width, int height, int sectionId)
{
	this->classId = classId;
	this->currentSectionId = sectionId;
	SetPosition(x, y);
	SetSize(width, height);
}

void CTileArea::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = x + width;
	top = y;
	bottom = y + height;
}

void CTileArea::Render(float offsetX, float offsetY)
{
	RenderBoundingBox();
	// what do you want to draw?
	// WHAT DO YOU WANT TO DRAW???
}

