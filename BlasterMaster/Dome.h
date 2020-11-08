#pragma once
#include "AnimatableObject.h"

class CDome : public CAnimatableObject
{
public:
    CDome() {};
    CDome(int classId, int x, int y, int animsId) : CAnimatableObject::CAnimatableObject(classId, x, y, animsId) {};
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CDome* LPDOME;
