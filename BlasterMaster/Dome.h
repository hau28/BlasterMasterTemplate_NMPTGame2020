#pragma once
#include "AnimatableObject.h"

// bound 8 12 23 18

class CDome : public CAnimatableObject
{
public:
    CDome() {};
    CDome(int classId, int x, int y, int animsId);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

typedef CDome* LPDOME;
