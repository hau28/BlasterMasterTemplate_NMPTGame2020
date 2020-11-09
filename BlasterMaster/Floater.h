#pragma once
#include "AnimatableObject.h"

class CFloater : public CAnimatableObject
{
public:
    CFloater() {};
    CFloater(int classId, int x, int y, int animsId);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CFloater* LPFLOATER;
