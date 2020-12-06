#pragma once
#include "AnimatableObject.h"
class CEnemy : public CAnimatableObject
{
public:
	CEnemy() {};
	CEnemy(int classId, int x, int y, int sectionId, int animsId);
};

