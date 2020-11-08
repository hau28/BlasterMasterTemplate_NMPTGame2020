#pragma once
#include "AnimatableObject.h"

class CFloater :
    public CAnimatableObject
{
public:
    CFloater() {};
    CFloater(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId) {};
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CFloater* LPFLOATER;
