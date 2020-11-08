#include "AnimatableObject.h"

CAnimatableObject::CAnimatableObject(int classId, int x, int y, int objAnimsId)
{
	this->classId = classId;
	SetPosition(x, y);

	LPOBJECT_ANIMATIONS objAnims = CObjectAnimationsLib::GetInstance()->Get(objAnimsId);
	animationHandlers = objAnims->GenerateAnimationHanlders();
}

void CAnimatableObject::Render()
{
	animationHandlers[state]->Render(x, y);
	animationHandlers[state]->Update();
}
