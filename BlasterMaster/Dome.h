#pragma once
#include "AnimatableObject.h"

class CDome : public CAnimatableObject
{
public:
    CDome() {};
    CDome(int classId, int x, int y, int animsId);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CDome* LPDOME;
